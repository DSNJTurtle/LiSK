## LiSK Tests ##

- The files "testing\_arprec.cpp" and "testing\_dprec.cpp" provide checks of the public high-level functions of LiSK in arbitrary-precision and double-precision, respectively. Meaning we compute `Li(n,x)` and `Li22(x,y)` for given points and weights `n` in all regions of the complex plane.
- The **Google Test** framework is utilised and is automatically downloaded from its git repository when executing **cmake**.
- Cross-check values for `Li(n,x)` and `Li22(x,y)` are provided by **GiNaC**. GiNaC is not part of LiSK or this testing suit and must be provided externally. It can be downloaded from [http://www.ginac.de](http://www.ginac.de). If GiNaC is not installed in the common search paths, the paths must be set via `ccmake`.
- The desired precision, relative error and weights `n` can be set at the top of each "testing\_*.cpp" file.