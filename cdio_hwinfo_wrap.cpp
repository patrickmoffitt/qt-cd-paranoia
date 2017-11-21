/*
  Copyright (C) 2017
  Patrick Moffitt

  Copyright (C) 2011
  Rocky Bernstein <rocky@gnu.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "cdio_hwinfo_wrap.hpp"

cdio_hwinfo_wrap::cdio_hwinfo_wrap() {}

std::array<char *, 3> cdio_hwinfo_wrap::get_members() {
    members = {
        info_ptr->psz_vendor,
        info_ptr->psz_model,
        info_ptr->psz_revision
    };
    return members;
}

void cdio_hwinfo_wrap::apply(
        std::function<void(std::function<void(std::string &s)> op, char *c)> apply_char_ptr,
        std::function<void(std::string &s)> op
        ) {
    for (auto i : get_members() ) {
        apply_char_ptr(op, i);
    }
}

