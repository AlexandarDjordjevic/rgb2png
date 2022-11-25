#pragma once
#include <cstdint>
#include <functional>
#include <png.h>
#include <span>
#include <vector>

class PngImage {
public:
  using DataReadyCb = std::function<void(std::span<uint8_t> data)>;

  PngImage(uint32_t width, uint32_t height);

  ~PngImage();

  void writeStart(DataReadyCb);

  void writeChunk(const std::span<uint8_t>);

  void writeEnd();

private:
  uint32_t m_width;
  uint32_t m_height;
  png_structp m_png;
  png_infop m_infoPtr;
  DataReadyCb m_dataReadyCb;
};
