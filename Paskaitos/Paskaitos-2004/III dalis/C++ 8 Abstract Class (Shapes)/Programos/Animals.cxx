//////////////////////////////////////////////////////////////////////////////
//
//  Abstract: Example for multiple inheritance with virtual base classes
//
//  Output from the program for different combinations of virtual and
//  non-virtual base classes is appended in comments at the end.
//
//  Concentrate on the correct/incorrect behaviour of the classes
//  depending on whether a virtual base class is specified or not.
//  This can be seen from the "age" output, there should be only one
//  storage location for "age" (i.e. the age should be the same).
//
//  The output for "sizeof" demonstrates how the size of the object
//  changes, but is machine and compiler-dependent.  It also gets
//  really confusing quickly as there are two effects at work:
//    - storage is saved because there are no multiple copies of "age"
//    - more storage is needed because objects with virtual base classes
//      need to carry pointers to the virtual data and function members.
//  Don't worry about understanding this in detail!!!
//
//
//  Version  Date         Author            Purpose
//  1.0.1    12-Mar-1998  Thomas Wieland    Added some more comments
//  1.0      05-Mar-1998  Thomas Wieland    Created
//
//////////////////////////////////////////////////////////////////////////////


#include  <iostream.h>


class Animal
{
  public:
    int age;
    // ....
};

class Mammal : virtual public Animal
{
  public:
    int  litter_size;
    // ....
};

class Pet : virtual public Animal
{
  public:
    char*  name;
    // ....
};

// both a Mammal and a Pet; both should be virtual!
class HouseCat :  virtual public Mammal, virtual public Pet
{
  // without virtual: has Mammal::age and Pet::age, both inherited from Animal
  // with    virtual: has only one age, Animal::age, which makes more sense
};


//
//  Exercise classes
//
main()
{
  Mammal    lion;
  Pet       fido;
  HouseCat  capucine;

  lion.age = 10;         // inherited from Animal
  lion.litter_size = 3;  // specific to Mammal

  fido.age = 1;          // inherited from Animal
  fido.name = "You are the Master";  // specific to Pet

  capucine.litter_size = 6;  // from Mammal
  capucine.name = "My cat";  // from Pet

  // References to 'age' are potentially ambiguous and may lead to errors:
  // - without any virtual base classes:
  //     error: ambiguous Animal::age and Animal::age (no virtual base)
  // - only Mammal or Pet use virtual base class:
  //     error: ambiguous Animal::age and Animal::age (one not in virtual base)
  //

  // Setting the different aliases of 'age' to different values allows us
  // to verify that they really all refer to the same storage location.
  // Only the value set in the last assignment should be printed.
  capucine.Animal::age = 666;
  capucine.Mammal::age = 33;
  capucine.Pet::age = 14;
  capucine.age = 17;         // from Animal


  cout << "Sizeof(Animal)   = " << sizeof(Animal) << endl;
  cout << "Sizeof(Mammal)   = " << sizeof(Mammal) << endl;
  cout << "Sizeof(Pet)      = " << sizeof(Pet) << endl;
  cout << "Sizeof(HouseCat) = " << sizeof(HouseCat) << endl;

  cout << "Animal::age   = " << capucine.Animal::age << endl;
  cout << "Mammal::age   = " << capucine.Mammal::age << endl;
  cout << "Pet::age      = " << capucine.Pet::age << endl;
  cout << "HouseCat::age = " << capucine.age << endl;
}


/********************************** Output **********************************
 ***
 *** WARNING: Not for the faint of heart!
 ***


No virtual base class at all:

	Sizeof(Animal)   = 4
	Sizeof(Mammal)   = 8  <-- 'age' inherited & part of Mammal
	Sizeof(Pet)      = 8
	Sizeof(HouseCat) = 16 <-- two copies of 'age' inherited from Mammal & Pet

	// Animal::age is ambiguous
	Mammal::age   = 33    <-- incorrect: separate age for Mammal & Pet
	Pet::age      = 14
	// Housecat::age is ambiguous


Only Mammal (or Pet) uses virtual base class, Housecat doesn't:

	Sizeof(Animal)   = 4
	Sizeof(Mammal)   = 12    (8)  <-- 'age' not inherited, but need two pointers
	Sizeof(Pet)      = 8     (12)     to virtual data and function members
	Sizeof(HouseCat) = 20

	// Animal::age is ambiguous
	Mammal::age   = 33
	Pet::age      = 14
	// Housecat::age is ambiguous


Both Mammal and Pet use virtual base class, Housecat doesn't:

	Sizeof(Animal)   = 4
	Sizeof(Mammal)   = 12
	Sizeof(Pet)      = 12
	Sizeof(HouseCat) = 20 <-- one set of pointers to same virtual base class 

	Animal::age   = 17    <-- correct: only one 'age' exists, shared by all
	Mammal::age   = 17
	Pet::age      = 17
	HouseCat::age = 17


Housecat declares Mammal (or Pet) as virtual base class:

	- Neither Mammal nor Pet use virtual base class:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 8
		Sizeof(Pet)      = 8
		Sizeof(HouseCat) = 20  <-- set of pointers to virtual Mammal (Pet)

		// Animal::age is ambiguous
		Mammal::age   = 33
		Pet::age      = 14
		// Housecat::age is ambiguous

	- Mammal (or Pet) does not use virtual base, the other does:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 8   (12)
		Sizeof(Pet)      = 12  (8)
		Sizeof(HouseCat) = 24

		// Animal::age is ambiguous
		Mammal::age   = 33
		Pet::age      = 14
		// Housecat::age is ambiguous

	- Mammal (or Pet) uses virtual base, the other doesn't:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 12  (8)
		Sizeof(Pet)      = 8   (12)
		Sizeof(HouseCat) = 28

		// Animal::age is ambiguous
		Mammal::age   = 33
		Pet::age      = 14
		// Housecat::age is ambiguous

	- Both Mammal and Pet use virtual base classes:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 12
		Sizeof(Pet)      = 12
		Sizeof(HouseCat) = 24

		Animal::age   = 17
		Mammal::age   = 17
		Pet::age      = 17
		HouseCat::age = 17


Housecat declares Mammal and Pet as virtual base classes:

	- Neither Mammal nor Pet use virtual base class:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 8
		Sizeof(Pet)      = 8
		Sizeof(HouseCat) = 24

		// Animal::age is ambiguous
		Mammal::age   = 33
		Pet::age      = 14
		// Housecat::age is ambiguous

	- Mammal (or Pet) does not use virtual base, the other does:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 8   (12)
		Sizeof(Pet)      = 12  (8)
		Sizeof(HouseCat) = 32

		// Animal::age is ambiguous
		Mammal::age   = 33
		Pet::age      = 14
		// Housecat::age is ambiguous

	- Both Mammal and Pet use virtual base classes:

		Sizeof(Animal)   = 4
		Sizeof(Mammal)   = 12
		Sizeof(Pet)      = 12
		Sizeof(HouseCat) = 36

		Animal::age   = 17
		Mammal::age   = 17
		Pet::age      = 17
		HouseCat::age = 17

 ****************************************************************************/

