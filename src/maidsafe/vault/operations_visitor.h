/*  Copyright 2012 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#ifndef MAIDSAFE_VAULT_OPERATIONS_VISITOR_H_
#define MAIDSAFE_VAULT_OPERATIONS_VISITOR_H_

#include "maidsafe/common/types.h"
#include "maidsafe/nfs/types.h"
#include "maidsafe/vault/types.h"
#include "maidsafe/common/node_id.h"

namespace maidsafe {

namespace vault {

namespace detail {

template <typename ServiceHandlerType>
class MaidManagerPutVisitor : public boost::static_visitor<> {
 public:
  MaidManagerPutVisitor(ServiceHandlerType* service, NonEmptyString content, NodeId sender,
                        Identity pmid_hint, nfs::MessageId message_id)
      : kService_(service),
        kContent_(std::move(content)),
        kSender_(std::move(sender)),
        kPmidHint_(std::move(pmid_hint)),
        kMessageId_(std::move(message_id)) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePut(MaidName(kSender_), Name::data_type(data_name, kContent_),
                                  kPmidHint_, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const NonEmptyString kContent_;
  const NodeId kSender_;
  const Identity kPmidHint_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType>
class DataManagerPutVisitor : public boost::static_visitor<> {
 public:
  DataManagerPutVisitor(ServiceHandlerType* service, NonEmptyString content, Identity maid_name,
                        Identity pmid_name, nfs::MessageId message_id)
      : kService_(service),
        kContent_(std::move(content)),
        kMaidName_(std::move(maid_name)),
        kPmidName_(std::move(pmid_name)),
        kMessageId_(std::move(message_id)) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePut(Name::data_type(data_name, kContent_), kMaidName_, kPmidName_,
                                 kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const NonEmptyString kContent_;
  const MaidName kMaidName_;
  const PmidName kPmidName_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType>
class PmidManagerPutVisitor : public boost::static_visitor<> {
 public:
  PmidManagerPutVisitor(ServiceHandlerType* service, const NonEmptyString& content,
                        const Identity& pmid_name, const nfs::MessageId& message_id)
      : kService_(service), kContent_(content), kPmidName_(pmid_name), kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePut(Name::data_type(data_name, kContent_), kPmidName_, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const NonEmptyString kContent_;
  const PmidName kPmidName_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType>
class PmidNodePutVisitor : public boost::static_visitor<> {
 public:
  PmidNodePutVisitor(ServiceHandlerType* service, const NonEmptyString& content,
                     const nfs::MessageId& message_id)
      : kService_(service), kContent_(content), kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePut(Name::data_type(data_name, kContent_), kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const NonEmptyString kContent_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType>
class PutResponseFailureVisitor : public boost::static_visitor<> {
 public:
  PutResponseFailureVisitor(ServiceHandlerType* service, const Identity& pmid_node,
                            const maidsafe_error& return_code, const nfs::MessageId& message_id)
      : kService_(service),
        kPmidNode_(pmid_node),
        kReturnCode_(return_code),
        kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutFailure(data_name, kPmidNode_, kMessageId_,
                                         maidsafe_error(kReturnCode_));
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidNode_;
  const maidsafe_error kReturnCode_;
  const nfs::MessageId kMessageId_;
};


template <typename ServiceHandlerType>
class MaidManagerPutResponseFailureVisitor : public boost::static_visitor<> {
 public:
  MaidManagerPutResponseFailureVisitor(ServiceHandlerType* service, const MaidName& maid_node,
                                       const maidsafe_error& return_code,
                                       const nfs::MessageId& message_id)
      : kService_(service),
        kMaidNode_(maid_node),
        kReturnCode_(return_code),
        kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutFailure(kMaidNode_, data_name, kReturnCode_, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const MaidName kMaidNode_;
  const maidsafe_error kReturnCode_;
  const nfs::MessageId kMessageId_;
};



template <typename ServiceHandlerType>
class PutResponseSuccessVisitor : public boost::static_visitor<> {
 public:
  PutResponseSuccessVisitor(ServiceHandlerType* service, const Identity& pmid_node,
                            const nfs::MessageId& message_id)
      : kService_(service), kPmidNode_(pmid_node), kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutResponse<typename Name::data_type>(data_name, kPmidNode_,
                                                                    kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidNode_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType>
class MaidManagerPutResponseVisitor : public boost::static_visitor<> {
 public:
  MaidManagerPutResponseVisitor(ServiceHandlerType* service, const Identity& maid_node,
                                const int32_t& cost)
      : kService_(service), kMaidNode_(maid_node), kCost_(cost) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutResponse(kMaidNode_, data_name, kCost_);
  }

 private:
  ServiceHandlerType* const kService_;
  const MaidName kMaidNode_;
  const int32_t kCost_;
};

template <typename ServiceHandlerType>
class DataManagerPutResponseVisitor : public boost::static_visitor<> {
 public:
  DataManagerPutResponseVisitor(ServiceHandlerType* service, const PmidName& pmid_node,
                                int32_t size, const nfs::MessageId& message_id)
      : kService_(service), kPmidNode_(pmid_node), kSize_(size), kMessageId_(message_id) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutResponse<Name::data_type>(data_name, kPmidNode_, kSize_,
                                                           kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidNode_;
  const int32_t kSize_;
  const nfs::MessageId kMessageId_;
};

template <typename ServiceHandlerType, typename Requestor>
class GetRequestVisitor : public boost::static_visitor<> {
 public:
  GetRequestVisitor(ServiceHandlerType* service, Requestor requestor, nfs::MessageId message_id)
      : kService_(service),
        kRequestor_(std::move(requestor)),
        kMessageId_(std::move(message_id)) {}

  template <typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandleGet<typename Name::data_type, Requestor>(data_name, kRequestor_,
                                                                       kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const Requestor kRequestor_;
  const nfs::MessageId kMessageId_;
};

template<typename ServiceHandlerType>
class DataManagerSendDeleteVisitor : public boost::static_visitor<> {
 public:
  DataManagerSendDeleteVisitor(ServiceHandlerType* service, const PmidName& pmid_node,
                               const nfs::MessageId& message_id)
      : kService_(service), kPmidNode_(pmid_node), kMessageId_(message_id) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template SendDeleteRequest<typename Name::data_type>(kPmidNode_, data_name,
                                                                    kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidNode_;
  const nfs::MessageId& kMessageId_;
};

template<typename ServiceHandlerType>
class PmidNodeDeleteVisitor : public boost::static_visitor<> {
 public:
  PmidNodeDeleteVisitor(ServiceHandlerType* service) : kService_(service) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandleDelete<typename Name::data_type>(data_name);
  }

 private:
  ServiceHandlerType* const kService_;
};

template<typename ServiceHandlerType>
class MaidManagerDeleteVisitor : public boost::static_visitor<> {
 public:
  MaidManagerDeleteVisitor(ServiceHandlerType* service, const MaidName& maid_name,
                           const nfs::MessageId& message_id)
      : kService_(service), kMaidName_(maid_name), kMessageId_(message_id) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandleDelete<typename Name::data_type>(kMaidName_, data_name, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const MaidName kMaidName_;
  const nfs::MessageId kMessageId_;
};

template<typename ServiceHandlerType>
class DataManagerDeleteVisitor : public boost::static_visitor<> {
 public:
  DataManagerDeleteVisitor(ServiceHandlerType* service, const nfs::MessageId& message_id)
      : kService_(service), kMessageId_(message_id) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandleDelete<typename Name::data_type>(data_name, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const nfs::MessageId kMessageId_;
};

template<typename ServiceHandlerType>
class PmidManagerDeleteVisitor : public boost::static_visitor<> {
 public:
  PmidManagerDeleteVisitor(ServiceHandlerType* service, const PmidName& pmid_name,
                           const nfs::MessageId& message_id)
      : kService_(service), kPmidName_(pmid_name), kMessageId_(message_id) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandleDelete<typename Name::data_type>(kPmidName_, data_name, kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidName_;
  const nfs::MessageId kMessageId_;
};

template<typename ServiceHandlerType>
class PmidManagerPutResponseVisitor : public boost::static_visitor<> {
 public:
  PmidManagerPutResponseVisitor(ServiceHandlerType* service, int32_t size,
                                const PmidName& pmid_name, const nfs::MessageId& message_id)
      : kService_(service), kPmidName_(pmid_name), kSize_(size), kMessageId_(message_id) {}

  template<typename Name>
  void operator()(const Name& data_name) {
    kService_->template HandlePutResponse<typename Name::data_type>(data_name, kSize_, kPmidName_,
                                                                    kMessageId_);
  }

 private:
  ServiceHandlerType* const kService_;
  const PmidName kPmidName_;
  const int32_t kSize_;
  const nfs::MessageId kMessageId_;
};



}  // namespace detail

}  // namespace vault

}  // namespace maidsafe

#include "maidsafe/vault/utils-inl.h"

#endif  // MAIDSAFE_VAULT_OPERATIONS_VISITOR_H_
