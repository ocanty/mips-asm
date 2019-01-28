//
// Created by ocanty on 28/01/19.
//

#include "token_type.hpp"

namespace as {

const std::string &get_name_for_token(const token_type &tk) {
    return token_type_name.at(tk);
}

}