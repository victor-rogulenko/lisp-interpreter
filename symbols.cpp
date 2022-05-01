#include <symbols.h>

True::True() : Symbol("#t") {
}

False::False() : Symbol("#f") {
}

bool False::IsFalse() const {
    return true;
}
