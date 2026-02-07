{ lib, stdenv, cmake, boost, libpng, libjpeg }:

stdenv.mkDerivation {
  pname = "mapcrafter";
  version = "unstable";

  src = lib.cleanSourceWith {
    src = lib.cleanSource ./.;
    filter = name: type: baseNameOf name != "build";
  };

  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost libpng libjpeg ];
}
