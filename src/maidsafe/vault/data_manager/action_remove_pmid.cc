/*  Copyright 2013 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.novinet.com/license

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#include "maidsafe/vault/data_manager/action_remove_pmid.h"

#include "maidsafe/common/error.h"

#include "maidsafe/vault/data_manager/action_remove_pmid.pb.h"
#include "maidsafe/vault/maid_manager/metadata.h"
#include "maidsafe/vault/data_manager/value.h"


namespace maidsafe {

namespace vault {

ActionDataManagerRemovePmid::ActionDataManagerRemovePmid(const PmidName& pmid_name)
    : kPmidName(pmid_name) {}

ActionDataManagerRemovePmid::ActionDataManagerRemovePmid(const std::string& serialised_action)
    : kPmidName([&serialised_action]()->PmidName {
        protobuf::ActionDataManagerRemovePmid action_remove_pmid_proto;
        if (!action_remove_pmid_proto.ParseFromString(serialised_action))
          ThrowError(CommonErrors::parsing_error);
        return PmidName(Identity(action_remove_pmid_proto.pmid_name()));
      }()) {}

ActionDataManagerRemovePmid::ActionDataManagerRemovePmid(const ActionDataManagerRemovePmid& other)
    : kPmidName(other.kPmidName) {}

ActionDataManagerRemovePmid::ActionDataManagerRemovePmid(ActionDataManagerRemovePmid&& other)
    : kPmidName(std::move(other.kPmidName)) {}

std::string ActionDataManagerRemovePmid::Serialise() const {
  protobuf::ActionDataManagerRemovePmid action_remove_pmid_proto;
  action_remove_pmid_proto.set_pmid_name(kPmidName.value.string());
  return action_remove_pmid_proto.SerializeAsString();
}

void ActionDataManagerRemovePmid::operator()(boost::optional<DataManagerValue>& value) const {
  if (!value)
    ThrowError(CommonErrors::invalid_parameter);
  value->RemovePmid(kPmidName);
}

bool operator==(const ActionDataManagerRemovePmid& lhs, const ActionDataManagerRemovePmid& rhs) {
  return lhs.kPmidName == rhs.kPmidName;
}

bool operator!=(const ActionDataManagerRemovePmid& lhs, const ActionDataManagerRemovePmid& rhs) {
  return !operator==(lhs, rhs);
}

}  // namespace vault

}  // namespace maidsafe
