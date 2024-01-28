// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: sender_service.proto

#include "sender_service.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;

inline constexpr Frame::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : data_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR Frame::Frame(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct FrameDefaultTypeInternal {
  PROTOBUF_CONSTEXPR FrameDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~FrameDefaultTypeInternal() {}
  union {
    Frame _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 FrameDefaultTypeInternal _Frame_default_instance_;
static ::_pb::Metadata file_level_metadata_sender_5fservice_2eproto[1];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_sender_5fservice_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_sender_5fservice_2eproto = nullptr;
const ::uint32_t TableStruct_sender_5fservice_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::Frame, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::Frame, _impl_.data_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::Frame)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::_Frame_default_instance_._instance,
};
const char descriptor_table_protodef_sender_5fservice_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\024sender_service.proto\032\033google/protobuf/"
    "empty.proto\"\025\n\005Frame\022\014\n\004data\030\001 \001(\0142\322\001\n\rS"
    "enderService\022F\n\024StartCapturingFrames\022\026.g"
    "oogle.protobuf.Empty\032\026.google.protobuf.E"
    "mpty\0222\n\020AcquireNextFrame\022\026.google.protob"
    "uf.Empty\032\006.Frame\022E\n\023StopCapturingFrames\022"
    "\026.google.protobuf.Empty\032\026.google.protobu"
    "f.Emptyb\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_sender_5fservice_2eproto_deps[1] =
    {
        &::descriptor_table_google_2fprotobuf_2fempty_2eproto,
};
static ::absl::once_flag descriptor_table_sender_5fservice_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_sender_5fservice_2eproto = {
    false,
    false,
    295,
    descriptor_table_protodef_sender_5fservice_2eproto,
    "sender_service.proto",
    &descriptor_table_sender_5fservice_2eproto_once,
    descriptor_table_sender_5fservice_2eproto_deps,
    1,
    1,
    schemas,
    file_default_instances,
    TableStruct_sender_5fservice_2eproto::offsets,
    file_level_metadata_sender_5fservice_2eproto,
    file_level_enum_descriptors_sender_5fservice_2eproto,
    file_level_service_descriptors_sender_5fservice_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_sender_5fservice_2eproto_getter() {
  return &descriptor_table_sender_5fservice_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_sender_5fservice_2eproto(&descriptor_table_sender_5fservice_2eproto);
// ===================================================================

class Frame::_Internal {
 public:
};

Frame::Frame(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:Frame)
}
inline PROTOBUF_NDEBUG_INLINE Frame::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : data_(arena, from.data_),
        _cached_size_{0} {}

Frame::Frame(
    ::google::protobuf::Arena* arena,
    const Frame& from)
    : ::google::protobuf::Message(arena) {
  Frame* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:Frame)
}
inline PROTOBUF_NDEBUG_INLINE Frame::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : data_(arena),
        _cached_size_{0} {}

inline void Frame::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
Frame::~Frame() {
  // @@protoc_insertion_point(destructor:Frame)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void Frame::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.data_.Destroy();
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void Frame::Clear() {
// @@protoc_insertion_point(message_clear_start:Frame)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.data_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* Frame::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> Frame::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_Frame_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // bytes data = 1;
    {::_pbi::TcParser::FastBS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(Frame, _impl_.data_)}},
  }}, {{
    65535, 65535
  }}, {{
    // bytes data = 1;
    {PROTOBUF_FIELD_OFFSET(Frame, _impl_.data_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kBytes | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* Frame::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Frame)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // bytes data = 1;
  if (!this->_internal_data().empty()) {
    const std::string& _s = this->_internal_data();
    target = stream->WriteBytesMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Frame)
  return target;
}

::size_t Frame::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Frame)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes data = 1;
  if (!this->_internal_data().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                    this->_internal_data());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData Frame::_class_data_ = {
    Frame::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* Frame::GetClassData() const {
  return &_class_data_;
}

void Frame::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<Frame*>(&to_msg);
  auto& from = static_cast<const Frame&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Frame)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_data().empty()) {
    _this->_internal_set_data(from._internal_data());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void Frame::CopyFrom(const Frame& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Frame)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool Frame::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* Frame::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void Frame::InternalSwap(Frame* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.data_, &other->_impl_.data_, arena);
}

::google::protobuf::Metadata Frame::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_sender_5fservice_2eproto_getter, &descriptor_table_sender_5fservice_2eproto_once,
      file_level_metadata_sender_5fservice_2eproto[0]);
}
// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
