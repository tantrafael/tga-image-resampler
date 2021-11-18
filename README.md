# TGA image resampler

Command line tool for scaling and resampling [Truevision TGA](https://en.wikipedia.org/wiki/Truevision_TGA) images.

## Usage
    /path/to/executable [options]
      --src [source filename]       Source TGA image to load.
      --dst [destination filename]  Destination TGA image to save.
      --width [integer]             Sets the width of the output image.
      --height [integer]            Sets the height of the output image.
      --kernel [integer]            Specifies the kernel type.
          1: Bicubic
          2: Catmull-Rom
          3: Mitchell-Netravali
          4: Cardinal
          5: B-Spline
          6: Lanczos
          7: Lanczos-2
          8: Lanczos-3
          9: Lanczos-4
         10: Lanczos-5

Example:

    /path/to/executable \
      --src '/path/to/source-image.tga' \
      --dst '/path/to/destination-image.tga' \
      --width 640 \
      --height 480 \
      --kernel 8 \
      /

## Thoughts and decisions

### Assumptions
Despite game code’s insatiable demand for swiftness, I for some reason had image quality above all in mind. Imagining rescaling of the classical image format taking place somewhere other than real-time, or at least not in critical gameplay, I decided to focus on the visual perception of the images when shown to the player. Someway I envisioned continuous-tone depictions rather than line or pixel art.

### Ambitions
First and foremost I wanted to write a program that’s easy to read and understand, with clean code and modern C++. I also felt like laying the ground for a basic TGA image resizing tool, letting users set the desired output dimensions and selecting a resampling algorithm from a fairly useful list. I’ve therefore tried to make it clear and easy to extend it, supporting more TGA image types and adding algorithms, without further changes to existing code.

### Images types
Without deeper reflection or probability assessment I decided that the TGA images in question would most likely be uncompressed, 24-bit color (true color). Thus, I chose to implement resizing for that format to begin with, disregarding run-length encoded, color-mapped, grayscale and transparent images for the time being.

### Resampling algorithms
My first thought went to bicubic interpolation, which in my own experience of resizing photos gives appreciably smooth results for scaling both up and down. Research confirmed it to be one of the most popular filter functions, and the default for many image processing programs, because it is usually considered a good neutral default. Also, the bicubic algorithm is easily used to derive the Catmull-Rom, Mitchell-Netravali, Cardinal and B-Spline variants. This convinced me it was a good choice to start implementing.

Much reading about image resizing and signal processing also led me to the Lanczos algorithm, generally considered a very high-quality resampler, also recommended for both enlarging and shrinking, especially its Elliptical Weighted Averaging (EWA) version. It is usually slightly sharper than bicubic interpolation, but might produce slightly more ringing. Seeming like an even better choice than bicubic, I went for Lanczos as the next implementation. Altering a coefficient in the Lanczos algorithm, also gave me the variants I call Lanczos-2, Lanczos-3, Lanczos-4, Lanczos-5.

Reminding myself of the given task of halving the image size, I realized it’s worth considering the weakness of the above and related algorithms that sample a specific number of pixels. When downscaling more than twice, they will sample non-adjacent pixels, which results in losing data, and causes rough results. So, I’m curious about trying box sampling for shrinking by integral factors. I’ve also heard and read some about the discrete cosine transform (DCT) which apparently can be used for scaling images extremely well and quickly.

I made the decision to implement resampling in two separate passes, horizontal and vertical, thus restricting myself to separable filters.

### Code
I’ve programmed on a Mac, using Xcode 12.5, Clang 12.0.5, crossing fingers it will build in other compilers. I simply dropped the source code in a folder, assuming people work in Windows and Visual Studio, and are better off spinning up a new C++ console app project. But I will happily supply Xcode project files and MacOS executables to anyone interested.

### Time flies
I found this an extraordinarily interesting assignment, diving into a lot of image resampling and signal processing. I’ve been completely immersed, and would love to just keep going. Given more time, I would make it work for all TGA image types, run-length encoded, color-mapped, grayscale and transparent. I would implement more filters, e.g. nearest, average, bilinear, Gaussian, Hermite and what have you. I would refactor, reducing the excessive amount of method arguments. I would put further effort into naming and commenting things. I would look over the use of integer types. I would do a lot more. I would make it perfect.
