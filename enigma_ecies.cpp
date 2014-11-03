#include "enigma_ecies.hpp"

#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/asn.h>
#include <cryptopp/oids.h>

#include <iostream>
using namespace std;

using namespace CryptoPP;

AutoSeededRandomPool rng;

void enigma_ecies_public::encrypt (const std::string & infile, const std::string & outfile) const
{
   FileSource(infile.c_str(), true,
      new PK_EncryptorFilter(rng, publicKey_,
         new FileSink(outfile.c_str())));
}

void enigma_ecies_public::save (const std::string & filename) const
{
   // Create sink object to stream data into
   FileSink sink (filename.c_str());
   // Save key
   publicKey_.GetPublicKey().Save(sink);
}

void enigma_ecies_public::load (const std::string & filename)
{
   FileSource source (filename.c_str(), true);
   publicKey_.AccessPublicKey().Load(source);
}

enigma_ecies_private::enigma_ecies_private () : 
   privateKey_(rng, ASN1::secp256r1())
{
   publicKey_ = privateKey_;
}

void enigma_ecies_private::decrypt (const std::string & infile, const std::string & outfile) const
{
   FileSource(infile.c_str(), true,
      new PK_DecryptorFilter(rng, privateKey_,
         new FileSink(outfile.c_str())));
}

void enigma_ecies_private::save (const std::string & filename) const
{
   // Create sink object to stream data into
   FileSink sink (filename.c_str());
   // Save key
   privateKey_.GetPrivateKey().Save(sink);
}

void enigma_ecies_private::load (const std::string & filename)
{
   FileSource source (filename.c_str(),true);
   privateKey_.AccessPrivateKey().Load(source);
}

ICryptosystem * newECIESPrivateKey ()
{
   enigma_ecies_private * output = new enigma_ecies_private();
   return new enigma_ecies_private();
}
