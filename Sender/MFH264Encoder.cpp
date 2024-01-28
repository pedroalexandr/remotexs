#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Winmm.lib")

// std
#include <iostream>

// Windows
#include <windows.h>
#include <atlbase.h>

// DirectX
#include <d3d11.h>

// Media Foundation
#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <Codecapi.h>
#include <mftransform.h>

#include "MFH264Encoder.h"

// Simplified error handling via macros
#ifdef DEBUG
#define ON_ERROR __debugbreak(); return false;
#else
#define ON_ERROR system("pause"); return false;
#endif
#define CHECK(x, err) if (!x) { std::cerr << err << std::endl; ON_ERROR }
#define CHECK_HR(x, err) if (FAILED(x)) { std::cerr << err << std::endl; ON_ERROR }

using namespace remotexs;

CComPtr<IMFTransform> transform;
CComQIPtr<IMFMediaEventGenerator> eventGen;
DWORD inputStreamID;
DWORD outputStreamID;

bool MFH264Encoder::Initialize(
    CComPtr<ID3D11Device>& device,
    CComPtr<IMFDXGIDeviceManager>& device_manager,
    UINT32 frame_width,
    UINT32 frame_height
) {
    HRESULT hr;

    CComPtr<IMFAttributes> attributes;

    // ------------------------------------------------------------------------
    // Initialize Media Foundation & COM
    // ------------------------------------------------------------------------

    CHECK_HR(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE),
        "COM initialisation failed.");

    hr = MFStartup(MF_VERSION);
    CHECK_HR(hr, "Failed to start Media Foundation");

    // Create device manager
    UINT resetToken;
    hr = MFCreateDXGIDeviceManager(&resetToken, &device_manager.p);
    CHECK_HR(hr, "Failed to create DXGIDeviceManager");

    hr = device_manager->ResetDevice(device, resetToken);
    CHECK_HR(hr, "Failed to assign D3D device to device manager");

    // Find encoder
    CComHeapPtr<IMFActivate*> activateRaw;
    UINT32 activateCount = 0;

    // h264 output
    MFT_REGISTER_TYPE_INFO info = { MFMediaType_Video, MFVideoFormat_H264 };

    UINT32 flags = MFT_ENUM_FLAG_HARDWARE | MFT_ENUM_FLAG_SORTANDFILTER;

    hr = MFTEnumEx(
        MFT_CATEGORY_VIDEO_ENCODER,
        flags,
        NULL,
        &info,
        &activateRaw,
        &activateCount
    );

    CHECK_HR(hr, "Failed to enumerate MFTs");

    CHECK(activateCount, "No MFTs found");

    // Choose the first available encoder
    CComPtr<IMFActivate> activate = activateRaw[0];

    for (UINT32 i = 0; i < activateCount; i++)
        activateRaw[i]->Release();

    hr = activate->ActivateObject(IID_PPV_ARGS(&transform));
    CHECK_HR(hr, "Failed to activate MFT");

    hr = transform->GetAttributes(&attributes);
    CHECK_HR(hr, "Failed to get MFT attributes");

    // Get encoder name
    //UINT32 nameLength = 0;
    //std::wstring name;

    //hr = attributes->GetStringLength(MFT_FRIENDLY_NAME_Attribute, &nameLength);
    //CHECK_HR(hr, "Failed to get MFT name length");

    //// IMFAttributes::GetString returns a null-terminated wide string
    //name.resize(nameLength + 1);

    //hr = attributes->GetString(MFT_FRIENDLY_NAME_Attribute, &name[0], name.size(), &nameLength);
    //CHECK_HR(hr, "Failed to get MFT name");

    //name.resize(nameLength);

    //std::wcout << name << std::endl;

    hr = attributes->SetUINT32(MF_TRANSFORM_ASYNC_UNLOCK, TRUE);
    CHECK_HR(hr, "Failed to unlock MFT");

    eventGen = transform;
    CHECK(eventGen, "Failed to QI for event generator");

    hr = transform->GetStreamIDs(1, &inputStreamID, 1, &outputStreamID);

    if (hr == E_NOTIMPL)
    {
        inputStreamID = 0;
        outputStreamID = 0;
        hr = S_OK;
    }

    CHECK_HR(hr, "Failed to get stream IDs");

    // ------------------------------------------------------------------------
    // Configure hardware encoder MFT
    // ------------------------------------------------------------------------
    hr = transform->ProcessMessage(MFT_MESSAGE_SET_D3D_MANAGER, reinterpret_cast<ULONG_PTR>(device_manager.p));
    CHECK_HR(hr, "Failed to set D3D manager");

    hr = attributes->SetUINT32(MF_LOW_LATENCY, TRUE);
    CHECK_HR(hr, "Failed to set MF_LOW_LATENCY");

    CComPtr<IMFMediaType> outputType;
    hr = MFCreateMediaType(&outputType);
    CHECK_HR(hr, "Failed to create media type");

    hr = outputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    CHECK_HR(hr, "Failed to set MF_MT_MAJOR_TYPE on H264 output media type");

    hr = outputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
    CHECK_HR(hr, "Failed to set MF_MT_SUBTYPE on H264 output media type");

    hr = outputType->SetUINT32(MF_MT_AVG_BITRATE, 30000000);
    CHECK_HR(hr, "Failed to set average bit rate on H264 output media type");

    hr = MFSetAttributeSize(outputType, MF_MT_FRAME_SIZE, frame_width, frame_height);
    CHECK_HR(hr, "Failed to set frame size on H264 MFT out type");

    hr = MFSetAttributeRatio(outputType, MF_MT_FRAME_RATE, 60, 1);
    CHECK_HR(hr, "Failed to set frame rate on H264 MFT out type");

    hr = outputType->SetUINT32(MF_MT_INTERLACE_MODE, 2);
    CHECK_HR(hr, "Failed to set MF_MT_INTERLACE_MODE on H.264 encoder MFT");

    hr = outputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
    CHECK_HR(hr, "Failed to set MF_MT_ALL_SAMPLES_INDEPENDENT on H.264 encoder MFT");

    hr = transform->SetOutputType(outputStreamID, outputType, 0);
    CHECK_HR(hr, "Failed to set output media type on H.264 encoder MFT");

    CComPtr<IMFMediaType> inputType;
    hr = MFCreateMediaType(&inputType);
    CHECK_HR(hr, "Failed to create media type");

    for (DWORD i = 0;; i++)
    {
        inputType = nullptr;
        hr = transform->GetInputAvailableType(inputStreamID, i, &inputType);
        CHECK_HR(hr, "Failed to get input type");

        hr = inputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
        CHECK_HR(hr, "Failed to set MF_MT_MAJOR_TYPE on H264 MFT input type");

        hr = inputType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_NV12);
        CHECK_HR(hr, "Failed to set MF_MT_SUBTYPE on H264 MFT input type");

        hr = MFSetAttributeSize(inputType, MF_MT_FRAME_SIZE, frame_width, frame_height);
        CHECK_HR(hr, "Failed to set MF_MT_FRAME_SIZE on H264 MFT input type");

        hr = MFSetAttributeRatio(inputType, MF_MT_FRAME_RATE, 60, 1);
        CHECK_HR(hr, "Failed to set MF_MT_FRAME_RATE on H264 MFT input type");

        hr = transform->SetInputType(inputStreamID, inputType, 0);
        CHECK_HR(hr, "Failed to set input type");

        break;
    }

    DWORD mftStatus = 0;
    CHECK_HR(transform->GetInputStatus(0, &mftStatus), "Failed to get input status from H.264 MFT.");

    if (MFT_INPUT_STATUS_ACCEPT_DATA != mftStatus) {
        std::cerr << "E_NOTACCEPTING: " << mftStatus << std::endl;
        return false;
    }

    // Start encoder
    // Maybe this flush is NOT needed
    hr = transform->ProcessMessage(MFT_MESSAGE_COMMAND_FLUSH, NULL);
    CHECK_HR(hr, "Failed to process FLUSH command on H.264 MFT");

    hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, NULL);
    CHECK_HR(hr, "Failed to process BEGIN_STREAMING command on H.264 MFT");

    hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_START_OF_STREAM, NULL);
    CHECK_HR(hr, "Failed to process START_OF_STREAM command on H.264 MFT");

    return SUCCEEDED(hr);
}

bool MFH264Encoder::Encode(const CComPtr<ID3D11Texture2D>& surface, BYTE** encoded_frame_buffer)
{
    std::cout << "Encoding frame..." << std::endl;
    
    // Create buffer
    CComPtr<IMFMediaBuffer> inputBuffer;
    CHECK_HR(MFCreateDXGISurfaceBuffer(__uuidof(ID3D11Texture2D), surface, 0, FALSE, &inputBuffer), "Failed to create IMFMediaBuffer");

    // Create sample
    //MFT_OUTPUT_STREAM_INFO StreamInfo;
    CComPtr<IMFSample> sample;
    CHECK_HR(MFCreateSample(&sample), "Failed to create IMFSample");

    // maybe i need to set sample time & duration below
    CHECK_HR(sample->SetSampleTime(1706165754), "Failed to set sample time.");
    CHECK_HR(sample->SetSampleDuration(1000), "Failed to set sample duration.");

    //CHECK_HR(transform->GetOutputStreamInfo(0, &StreamInfo), "Failed to get output stream info from H264 MFT.");
    //CHECK_HR(MFCreateMemoryBuffer(StreamInfo.cbSize, &pBuffer), "Failed to create memory buffer."); // maybe it's needed
    CHECK_HR(sample->AddBuffer(inputBuffer), "Failed to add buffer to IMFSample");

    // change this to call either ProcessInput or ProcessOutput, maybe put back while loop

    /*HRESULT hr = transform->ProcessInput(inputStreamID, sample, 0);

    CHECK_HR(hr, "Failed to process input.");*/

    std::cout << "Frame encoded successfully!" << std::endl;

    std::cout << "Generating H264 buffer..." << std::endl;
    
    DWORD status;
    MFT_OUTPUT_DATA_BUFFER outputBuffer;
    outputBuffer.dwStreamID = outputStreamID;
    outputBuffer.dwStatus = 0;
    outputBuffer.pEvents = nullptr;
    outputBuffer.pSample = sample;

    CHECK_HR(transform->ProcessOutput(0, 1, &outputBuffer, &status), "ProcessOutput failed");

    outputBuffer.pSample->SetSampleTime(1706165754);
    outputBuffer.pSample->SetSampleDuration(1000);

    // Extract the H.264 buffer from the sample
    IMFMediaBuffer* mediaBuffer = nullptr;
    DWORD dwCurrentLen = 0;
    BYTE* pBuffer = nullptr;

    CHECK_HR(outputBuffer.pSample->ConvertToContiguousBuffer(&mediaBuffer), "Failed to convert to contiguous buffer");

    // Maybe it's NOT needed
    CHECK_HR(mediaBuffer->Lock(&pBuffer, nullptr, &dwCurrentLen), "Failed to lock media buffer");

    // pBuffer now points to the raw H.264 data
    // dwCurrentLen indicates the size of the H.264 data in bytes
    // Allocate memory for the encoded data and copy it
    *encoded_frame_buffer = new BYTE[dwCurrentLen];
    memcpy(*encoded_frame_buffer, pBuffer, dwCurrentLen);

    std::cout << "H264 buffer generated successfully!" << std::endl;

    CHECK_HR(mediaBuffer->Unlock(), "Failed to unlock media buffer");

    // Release resources
    if (outputBuffer.pSample) {
        outputBuffer.pSample->Release();
        outputBuffer.pSample = nullptr;
    }

    if (outputBuffer.pEvents) {
        outputBuffer.pEvents = nullptr;
        outputBuffer.pEvents->Release();
    }

    return true;
}

bool MFH264Encoder::StopEncoding()
{
    HRESULT hr;

    hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, NULL);
    CHECK_HR(hr, "Failed to process END_OF_STREAM command on H.264 MFT");

    hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_END_STREAMING, NULL);
    CHECK_HR(hr, "Failed to process END_STREAMING command on H.264 MFT");

    hr = transform->ProcessMessage(MFT_MESSAGE_COMMAND_FLUSH, NULL);
    CHECK_HR(hr, "Failed to process FLUSH command on H.264 MFT");

    return true;
}
