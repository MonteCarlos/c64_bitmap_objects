#include "vic2_charset.h"
/**
 * @brief Reads whole charset (256 chars) from inputstream "file"
 * @param file
 * @return file.fail()
 */
bool VIC2_Charset::fread (ifstream &file) {
    bool error = false;

    for (int i = 0; i < 256; ++i) {
        if (chars[i].fread (file)){
            error = true;
            break;
        }
    }

    return error;
}
