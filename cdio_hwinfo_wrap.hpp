#ifndef CDIO_HWINFO_WRAP_HPP
#define CDIO_HWINFO_WRAP_HPP
#include <array>
#include <functional>
#include <cdio/cdio.h>
#include <cdio/device.h>

/*
 * The combination of get_members() and appy() allows looping over hw_info_t.
 */

class cdio_hwinfo_wrap
{
    std::array<char *, 3> members;
public:
    cdio_hwinfo_wrap();
    cdio_hwinfo_t info;
    cdio_hwinfo_t *info_ptr = &info;
    std::array<char *, 3> get_members();
    void apply(std::function<void(std::function<void(std::string &s)> op, char *c)> apply_char_ptr,
               std::function<void(std::string &s)> op
               );
};

#endif // CDIO_HWINFO_WRAP_HPP
