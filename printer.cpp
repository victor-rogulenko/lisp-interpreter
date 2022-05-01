#include "printer.h"

void PrintTo(const std::shared_ptr<Object>& obj, std::ostream* out) {
    if (!obj) {
        *out << "()";
        return;
    }

    obj->PrintObjectToOstream(out);
}

std::string Print(const std::shared_ptr<Object>& obj) {
    std::stringstream ss;
    PrintTo(obj, &ss);
    return ss.str();
}
