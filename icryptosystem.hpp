#ifndef ICRYPTOSYSTEM_HPP
#define ICRYPTOSYSTEM_HPP

#include <string>

/**
 * Interface class for cryptographic objects (both public and private keys).
 */
class ICryptosystem
{
public:
   /// Deletes this object.
   virtual void destroy () = 0;
   
   /// Returns a copy of this object owned by the caller.
   virtual ICryptosystem * clone () const = 0;
   
   /**
    * Encrypts a file using this cryptosystem object's public key.
    * 
    * \param infile File to be encrypted.
    * \param outfile File in which to store encrypted data.
    */
   virtual void encrypt (const std::string & infile, const std::string & outfile) const = 0;
   
   /**
    * Decrypts a file using this cryptosystem object's public key.
    * 
    * \param infile File to be decrypted.
    * \param outfile File in which to store decrypted data.
    */
   virtual void decrypt (const std::string & infile, const std::string & outfile) const = 0;
   
   /**
    * Checks if a private key is present for a given public key.
    * 
    * \return TRUE if the object contains private key information, otherwise FALSE.
    */
   virtual bool has_private () const noexcept { return false; }
   
   /**
    * Saves public key information.
    * 
    * \param filename File to save the key to.
    */
   virtual void save (const std::string & filename) const = 0;
   
   /**
    * Loads a public key from file, overwriting this key.
    * 
    * \param filename File to load from.
    */
   virtual void load (const std::string & filename) = 0;
};

#endif
