//
// Created by dxve on 3.6.23.
//

#include "CDeposit.h"

CDeposit::CDeposit(const nlohmann::json &save_data) {

    CJsonCheck::checkIfContains(save_data, {"resource", "texture", "amount"});

    std::string saved_resource = save_data["resource"];
    std::string saved_texture = save_data["texture"];
    int saved_amount = save_data["amount"];

    resource = saved_resource;
    texture = saved_texture[0];
    amount = saved_amount;
}

CDeposit::CDeposit() : amount(-1), texture(' ') {
}

bool CDeposit::isEmpty() const {
    return resource.empty();
}

CDeposit::CDeposit(const std::string &resource, int amount, char texture)
                    : resource(resource), amount(amount), texture(texture){

}

