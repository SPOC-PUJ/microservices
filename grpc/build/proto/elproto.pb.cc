// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/elproto.proto
// Protobuf C++ Version: 5.26.1

#include "proto/elproto.pb.h"

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

inline constexpr MovingAverageResponse::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : output_{},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR MovingAverageResponse::MovingAverageResponse(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct MovingAverageResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MovingAverageResponseDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MovingAverageResponseDefaultTypeInternal() {}
  union {
    MovingAverageResponse _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MovingAverageResponseDefaultTypeInternal _MovingAverageResponse_default_instance_;

inline constexpr MovingAverageRequest::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : input_{},
        window_size_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR MovingAverageRequest::MovingAverageRequest(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct MovingAverageRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MovingAverageRequestDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~MovingAverageRequestDefaultTypeInternal() {}
  union {
    MovingAverageRequest _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MovingAverageRequestDefaultTypeInternal _MovingAverageRequest_default_instance_;
static ::_pb::Metadata file_level_metadata_proto_2felproto_2eproto[2];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_proto_2felproto_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_proto_2felproto_2eproto = nullptr;
const ::uint32_t
    TableStruct_proto_2felproto_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::MovingAverageRequest, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::MovingAverageRequest, _impl_.input_),
        PROTOBUF_FIELD_OFFSET(::MovingAverageRequest, _impl_.window_size_),
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::MovingAverageResponse, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::MovingAverageResponse, _impl_.output_),
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::MovingAverageRequest)},
        {10, -1, -1, sizeof(::MovingAverageResponse)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::_MovingAverageRequest_default_instance_._instance,
    &::_MovingAverageResponse_default_instance_._instance,
};
const char descriptor_table_protodef_proto_2felproto_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\023proto/elproto.proto\":\n\024MovingAverageRe"
    "quest\022\r\n\005input\030\001 \003(\001\022\023\n\013window_size\030\002 \001("
    "\005\"\'\n\025MovingAverageResponse\022\016\n\006output\030\001 \003"
    "(\0012`\n\027SignalProcessingService\022E\n\024Compute"
    "MovingAverage\022\025.MovingAverageRequest\032\026.M"
    "ovingAverageResponseb\006proto3"
};
static ::absl::once_flag descriptor_table_proto_2felproto_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_proto_2felproto_2eproto = {
    false,
    false,
    228,
    descriptor_table_protodef_proto_2felproto_2eproto,
    "proto/elproto.proto",
    &descriptor_table_proto_2felproto_2eproto_once,
    nullptr,
    0,
    2,
    schemas,
    file_default_instances,
    TableStruct_proto_2felproto_2eproto::offsets,
    file_level_metadata_proto_2felproto_2eproto,
    file_level_enum_descriptors_proto_2felproto_2eproto,
    file_level_service_descriptors_proto_2felproto_2eproto,
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
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_proto_2felproto_2eproto_getter() {
  return &descriptor_table_proto_2felproto_2eproto;
}
// ===================================================================

class MovingAverageRequest::_Internal {
 public:
};

MovingAverageRequest::MovingAverageRequest(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:MovingAverageRequest)
}
inline PROTOBUF_NDEBUG_INLINE MovingAverageRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : input_{visibility, arena, from.input_},
        _cached_size_{0} {}

MovingAverageRequest::MovingAverageRequest(
    ::google::protobuf::Arena* arena,
    const MovingAverageRequest& from)
    : ::google::protobuf::Message(arena) {
  MovingAverageRequest* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);
  _impl_.window_size_ = from._impl_.window_size_;

  // @@protoc_insertion_point(copy_constructor:MovingAverageRequest)
}
inline PROTOBUF_NDEBUG_INLINE MovingAverageRequest::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : input_{visibility, arena},
        _cached_size_{0} {}

inline void MovingAverageRequest::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.window_size_ = {};
}
MovingAverageRequest::~MovingAverageRequest() {
  // @@protoc_insertion_point(destructor:MovingAverageRequest)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void MovingAverageRequest::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
MovingAverageRequest::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(MovingAverageRequest, _impl_._cached_size_),
              false,
          },
          &MovingAverageRequest::MergeImpl,
          &MovingAverageRequest::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void MovingAverageRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:MovingAverageRequest)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.input_.Clear();
  _impl_.window_size_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* MovingAverageRequest::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<1, 2, 0, 0, 2> MovingAverageRequest::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    2, 8,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967292,  // skipmap
    offsetof(decltype(_table_), field_entries),
    2,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_MovingAverageRequest_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::MovingAverageRequest>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // int32 window_size = 2;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(MovingAverageRequest, _impl_.window_size_), 63>(),
     {16, 63, 0, PROTOBUF_FIELD_OFFSET(MovingAverageRequest, _impl_.window_size_)}},
    // repeated double input = 1;
    {::_pbi::TcParser::FastF64P1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(MovingAverageRequest, _impl_.input_)}},
  }}, {{
    65535, 65535
  }}, {{
    // repeated double input = 1;
    {PROTOBUF_FIELD_OFFSET(MovingAverageRequest, _impl_.input_), 0, 0,
    (0 | ::_fl::kFcRepeated | ::_fl::kPackedDouble)},
    // int32 window_size = 2;
    {PROTOBUF_FIELD_OFFSET(MovingAverageRequest, _impl_.window_size_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* MovingAverageRequest::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:MovingAverageRequest)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // repeated double input = 1;
  if (this->_internal_input_size() > 0) {
    target = stream->WriteFixedPacked(1, _internal_input(), target);
  }

  // int32 window_size = 2;
  if (this->_internal_window_size() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::
        WriteInt32ToArrayWithField<2>(
            stream, this->_internal_window_size(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:MovingAverageRequest)
  return target;
}

::size_t MovingAverageRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:MovingAverageRequest)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated double input = 1;
  {
    std::size_t data_size = std::size_t{8} *
        ::_pbi::FromIntSize(this->_internal_input_size())
    ;
    std::size_t tag_size = data_size == 0
        ? 0
        : 1 + ::_pbi::WireFormatLite::Int32Size(
                            static_cast<int32_t>(data_size))
    ;
    total_size += tag_size + data_size;
  }
  // int32 window_size = 2;
  if (this->_internal_window_size() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_window_size());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void MovingAverageRequest::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<MovingAverageRequest*>(&to_msg);
  auto& from = static_cast<const MovingAverageRequest&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:MovingAverageRequest)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_internal_mutable_input()->MergeFrom(from._internal_input());
  if (from._internal_window_size() != 0) {
    _this->_impl_.window_size_ = from._impl_.window_size_;
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void MovingAverageRequest::CopyFrom(const MovingAverageRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:MovingAverageRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool MovingAverageRequest::IsInitialized() const {
  return true;
}

void MovingAverageRequest::InternalSwap(MovingAverageRequest* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.input_.InternalSwap(&other->_impl_.input_);
        swap(_impl_.window_size_, other->_impl_.window_size_);
}

::google::protobuf::Metadata MovingAverageRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_proto_2felproto_2eproto_getter,
                                   &descriptor_table_proto_2felproto_2eproto_once,
                                   file_level_metadata_proto_2felproto_2eproto[0]);
}
// ===================================================================

class MovingAverageResponse::_Internal {
 public:
};

MovingAverageResponse::MovingAverageResponse(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:MovingAverageResponse)
}
inline PROTOBUF_NDEBUG_INLINE MovingAverageResponse::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : output_{visibility, arena, from.output_},
        _cached_size_{0} {}

MovingAverageResponse::MovingAverageResponse(
    ::google::protobuf::Arena* arena,
    const MovingAverageResponse& from)
    : ::google::protobuf::Message(arena) {
  MovingAverageResponse* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:MovingAverageResponse)
}
inline PROTOBUF_NDEBUG_INLINE MovingAverageResponse::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : output_{visibility, arena},
        _cached_size_{0} {}

inline void MovingAverageResponse::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
MovingAverageResponse::~MovingAverageResponse() {
  // @@protoc_insertion_point(destructor:MovingAverageResponse)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void MovingAverageResponse::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
MovingAverageResponse::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(MovingAverageResponse, _impl_._cached_size_),
              false,
          },
          &MovingAverageResponse::MergeImpl,
          &MovingAverageResponse::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void MovingAverageResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:MovingAverageResponse)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.output_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* MovingAverageResponse::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> MovingAverageResponse::_table_ = {
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
    &_MovingAverageResponse_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::MovingAverageResponse>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // repeated double output = 1;
    {::_pbi::TcParser::FastF64P1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(MovingAverageResponse, _impl_.output_)}},
  }}, {{
    65535, 65535
  }}, {{
    // repeated double output = 1;
    {PROTOBUF_FIELD_OFFSET(MovingAverageResponse, _impl_.output_), 0, 0,
    (0 | ::_fl::kFcRepeated | ::_fl::kPackedDouble)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* MovingAverageResponse::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:MovingAverageResponse)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // repeated double output = 1;
  if (this->_internal_output_size() > 0) {
    target = stream->WriteFixedPacked(1, _internal_output(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:MovingAverageResponse)
  return target;
}

::size_t MovingAverageResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:MovingAverageResponse)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated double output = 1;
  {
    std::size_t data_size = std::size_t{8} *
        ::_pbi::FromIntSize(this->_internal_output_size())
    ;
    std::size_t tag_size = data_size == 0
        ? 0
        : 1 + ::_pbi::WireFormatLite::Int32Size(
                            static_cast<int32_t>(data_size))
    ;
    total_size += tag_size + data_size;
  }
  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void MovingAverageResponse::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<MovingAverageResponse*>(&to_msg);
  auto& from = static_cast<const MovingAverageResponse&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:MovingAverageResponse)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_internal_mutable_output()->MergeFrom(from._internal_output());
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void MovingAverageResponse::CopyFrom(const MovingAverageResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:MovingAverageResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool MovingAverageResponse::IsInitialized() const {
  return true;
}

void MovingAverageResponse::InternalSwap(MovingAverageResponse* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.output_.InternalSwap(&other->_impl_.output_);
}

::google::protobuf::Metadata MovingAverageResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_proto_2felproto_2eproto_getter,
                                   &descriptor_table_proto_2felproto_2eproto_once,
                                   file_level_metadata_proto_2felproto_2eproto[1]);
}
// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::std::false_type _static_init_ PROTOBUF_UNUSED =
    (::_pbi::AddDescriptors(&descriptor_table_proto_2felproto_2eproto),
     ::std::false_type{});
#include "google/protobuf/port_undef.inc"
