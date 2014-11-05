#ifndef ENIGMA_ECIES_HPP
#define ENIGMA_ECIES_HPP

#include "icryptosystem.hpp"
#include <cryptopp/eccrypto.h>

class enigma_ecies_public: public ICryptosystem
{
public:
   /// Default constructor.
   enigma_ecies_public () {};
   /// Copy constructor.
   enigma_ecies_public (const enigma_ecies_public & other) : publicKey_(other.publicKey_) {}
   /// Move constructor.
   enigma_ecies_public (enigma_ecies_public && other) : publicKey_(std::move(other.publicKey_)) {}
   /// Copy assignment.
   const enigma_ecies_public & operator= (const enigma_ecies_public & other) { publicKey_ = other.publicKey_; }
   /// Move assignment.
   const enigma_ecies_public & operator= (enigma_ecies_public && other) { publicKey_ = std::move(other.publicKey_); }
   
   /// Deletes this object.
   virtual void destroy () { delete this; }
   
   /// Returns a copy of this object owned by the caller.
   virtual enigma_ecies_public * clone () const { return new enigma_ecies_public(*this); }
   
   virtual void encrypt (const std::string & infile, const std::string & outfile) const;
   
   virtual void decrypt (const std::string & infile, const std::string & outfile) const
   {
      // throw an exception here
   }
   
   virtual void save (const std::string & filename) const;
   virtual void load (const std::string & filename);
protected:
   enigma_ecies_public (CryptoPP::ECIES<CryptoPP::ECP>::Decryptor key) : publicKey_(key) {}
   CryptoPP::ECIES<CryptoPP::ECP>::Encryptor publicKey_;
};

class enigma_ecies_private: public enigma_ecies_public
{
public:
   /// Default constructor.
   enigma_ecies_private ();
   /// Copy constructor.
   enigma_ecies_private (const enigma_ecies_private & other) : enigma_ecies_public(other), privateKey_(other.privateKey_) {}
   /// Move constructor.
   enigma_ecies_private (enigma_ecies_private && other) : enigma_ecies_public(std::move(other)), privateKey_(std::move(other.privateKey_)) {}
   /// Copy assignment.
   const enigma_ecies_private & operator= (const enigma_ecies_private & other)
   {
      *((enigma_ecies_public *)(this)) = other;
      privateKey_ = other.privateKey_;
      return *this;
   }
   /// Move assignment.
   const enigma_ecies_private & operator= (enigma_ecies_private && other)
   {
      *((enigma_ecies_public *)(this)) = std::move(other);
      privateKey_ = std::move(other.privateKey_);
      return *this;
   }
   
   /// Deletes this object.
   virtual void destroy () { delete this; }
   
   /// Returns a copy of this object owned by the caller.
   virtual enigma_ecies_private * clone () const { return new enigma_ecies_private(*this); }
   
   virtual void decrypt (const std::string & infile, const std::string & outfile) const;
   virtual void save (const std::string & filename) const;
   virtual void load (const std::string & filename);
   
   virtual bool has_private () const noexcept { return true; }
private:
   CryptoPP::ECIES<CryptoPP::ECP>::Decryptor privateKey_;
};

/// Generates and returns a private key owned by the caller.
ICryptosystem * newECIESPrivateKey ();

#endif
