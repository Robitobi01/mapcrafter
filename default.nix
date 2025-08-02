{ stdenv, cmake, boost, libpng, libjpeg }:

stdenv.mkDerivation {
  pname = "mapcrafter";
  version = "unstable";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost libpng libjpeg ];
}
