#include "PngImage.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <span>
#include <thread>

std::vector<uint8_t> pngImageBuffer;

void writeFunct(std::span<uint8_t> data) {
  pngImageBuffer.insert(pngImageBuffer.end(), data.begin(), data.end());
}

int main() {
  auto height = 877;
  auto width = 638;
  auto bpp = 3;

  std::size_t rawImageSize = height * width * bpp;
  uint8_t *dataRgb = new uint8_t[rawImageSize];
  FILE *rawImage = fopen("test_file/scan.raw", "r");
  if (!rawImage) {
    std::cerr << "Failed to open file test_file/scan.raw" << std::endl;
    return EXIT_FAILURE;
  }
  fread(dataRgb, 1, rawImageSize, rawImage);
  fclose(rawImage);

  PngImage png(width, height);
  auto begin = std::chrono::high_resolution_clock::now();
  png.writeStart(&writeFunct);
  png.writeChunk({dataRgb, rawImageSize});
  png.writeEnd();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::cout << "Duration: " << duration.count() << "ms" << std::endl;

  FILE *outputImage = fopen("scan.png", "w+");
  if (!outputImage) {
    std::cerr << "Failed to create file scan.png" << std::endl;
    return EXIT_FAILURE;
  }

  fwrite(pngImageBuffer.data(), 1, pngImageBuffer.size(), outputImage);

  delete[] dataRgb;
  fclose(outputImage);
  return 0;
}
