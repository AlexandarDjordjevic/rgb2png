#include "PngImage.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

const auto BYTES_PER_PIXEL = 3;

} // namespace

PngImage::PngImage(uint32_t width, uint32_t height)
    : m_width(width), m_height(height),
      m_png(png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
                                    nullptr)),
      m_infoPtr(png_create_info_struct(m_png)) {
  png_set_IHDR(m_png, m_infoPtr, m_width, m_height, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
}

PngImage::~PngImage() { png_destroy_write_struct(&m_png, nullptr); }

void PngImage::writeStart(DataReadyCb callback) {
  m_dataReadyCb = callback;
  auto writeCallback = [](png_structp png_ptr, png_bytep data,
                          png_size_t length) {
    DataReadyCb *dptr = (DataReadyCb *)png_get_io_ptr(png_ptr);
    (*dptr)({data, length});
  };

  png_set_write_fn(m_png, &m_dataReadyCb, writeCallback, nullptr);
  png_write_info(m_png, m_infoPtr);
}

void PngImage::writeChunk(const std::span<uint8_t> data) {
  auto rowCount = data.size() / (m_width * BYTES_PER_PIXEL);
  assert(!(data.size() % (m_width * BYTES_PER_PIXEL)));
  uint8_t *rows[rowCount];
  for (size_t i = 0; i < rowCount; ++i) {
    rows[i] = data.data() + i * m_width * BYTES_PER_PIXEL;
  }
  std::cout << "Writing " << rowCount << " rows" << std::endl;
  png_write_rows(m_png, rows, rowCount);
}

void PngImage::writeEnd() { png_write_end(m_png, m_infoPtr); }
