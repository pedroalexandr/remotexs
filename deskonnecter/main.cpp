extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
}

class ScreenCapture {
public:
    ScreenCapture(int width, int height) : screenWidth(width), screenHeight(height) {
        av_register_all();
        avcodec_register_all();
        avformat_network_init();

        avformat_alloc_output_context2(&formatContext, NULL, NULL, "output.mp4");
        outputFormat = formatContext->oformat;

        codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        codecContext = avcodec_alloc_context3(codec);
        // Set codec parameters
        // ...

        // Open video output file
        if (!(outputFormat->flags & AVFMT_NOFILE)) {
            if (avio_open(&formatContext->pb, "output.mp4", AVIO_FLAG_WRITE) < 0) {
                // Error handling
            }
        }

        avformat_write_header(formatContext, NULL);
    }

    void CaptureAndEncode() {
        // Loop to capture screen and encode frames
        for (/* capture loop */) {
            AVFrame* frame = av_frame_alloc();
            // Capture screen data into 'frame'
            // ...

            // Set frame properties, like pts
            frame->pts = av_rescale_q(av_gettime(), (AVRational) { 1, 1000000 }, codecContext->time_base);

            // Send frame to encoder
            if (avcodec_send_frame(codecContext, frame) < 0) {
                // Error handling
            }

            // Receive encoded packet from encoder
            AVPacket pkt;
            av_init_packet(&pkt);
            pkt.data = NULL;
            pkt.size = 0;

            if (avcodec_receive_packet(codecContext, &pkt) == 0) {
                av_interleaved_write_frame(formatContext, &pkt);
                av_packet_unref(&pkt);
            }

            av_frame_free(&frame);
        }
    }

    ~ScreenCapture() {
        av_write_trailer(formatContext);
        avcodec_close(codecContext);
        avio_close(formatContext->pb);
        avformat_free_context(formatContext);
    }

private:
    int screenWidth;
    int screenHeight;
    AVFormatContext* formatContext;
    AVOutputFormat* outputFormat;
    AVCodec* codec;
    AVCodecContext* codecContext;
};

int main() {
    ScreenCapture capture(1920, 1080); // Set your screen resolution

    capture.CaptureAndEncode();

    return 0;
}