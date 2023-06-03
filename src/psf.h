#ifndef ODYSSEY_PSF_H
#define ODYSSEY_PSF_H

#include <stdint.h>

struct psf_header {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t num_glyph;
    uint32_t bytes_per_glyph;
    uint32_t height;
    uint32_t width;
};

#endif