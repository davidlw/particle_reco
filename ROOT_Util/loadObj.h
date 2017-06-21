//
// loadObj.h
// Utilities/
// 
// Created by Johann Gan on 6/2/17
// 
// 
// Template function. Opens a file and reads in a specified object. Returns a unique_ptr to the object.

//////////////////////////////////////

#ifndef LOADOBJ_H_
#define LOADOBJ_H_

//////////////////////////////////////

// Dependencies
#include <string>
#include <memory>
#include <iostream>

#include <TROOT.h>
#include <TFile.h>
#include <TDirectoryFile.h>

//////////////////////////////////////

using namespace std;

//////////////////////////////////////

// Supporting function templates
//
// Behavior determined via SFINAE tests


// releaseOwnership releases the object from the ownership of any enclosing directories.

// Implementation 1: if U::CloneTree exists.
// For when U is TTree of TNtuple.
template <class U>
unique_ptr<U> releaseOwnership_imp(unique_ptr<U> theObj, int highpriority1, int highpriority2, decltype(&U::CloneTree) test = 0)
{
	gROOT->cd();
	theObj.reset( (U*)theObj->CloneTree() );

	return theObj;
}

// Implementation 2: if U::CloneTree does not exist, but U::SetDirectory exists.
// Primarily for when U is a histogram class.
template <class U>
unique_ptr<U> releaseOwnership_imp(unique_ptr<U> theObj, int highpriority, long lowpriority, decltype(&U::SetDirectory) test = 0)
{
	theObj->SetDirectory(0);
	return theObj;
}

// Implementation 3: if neither U::CloneTree nor u::SetDirectory exist.
// Default method for various non-tree/non-histogram classes.
template <class U>
unique_ptr<U> releaseOwnership_imp(unique_ptr<U> theObj, long lowpriority1, long lowpriority2)
{
	return theObj;
}

// releaseOwnership definition
template <class U>
unique_ptr<U> releaseOwnership(unique_ptr<U> theObj)
{
	return releaseOwnership_imp( move(theObj), 0, 0 );
}

//====================================//

// doOpen opens a file of a specific class type if an Open method exists, and returns a nullptr if it doesn't.

// Implementation 1: U::Open exists
template <class U>
unique_ptr<U> doOpen_imp(const string& name, int highpriority, decltype(U::Open("")) test = 0)
{
	unique_ptr<U> newObj( (U*)U::Open(name.c_str()) );

	return newObj;
}

// Implementation 2: U::Open does not exist
template <class U>
unique_ptr<U> doOpen_imp(const string& name, long lowpriority)
{
	cout << "ERROR: Specified class type does not have method: Open().\n";
	return nullptr;
}

// doOpen definition
template <class U>
unique_ptr<U> doOpen(const string& name)
{
	return doOpen_imp<U>(name, 0);
}

//////////////////////////////////////



//Actual definition of loadObj

template <class T>
unique_ptr<T> loadObj(const string& objName, const string& fileName = "", const string& directoryName = "")
{
	// Opens a file and reads in a specified object. Returns a unique_ptr to the object.
	//
	// directoryName and fileName are optional parameters. They default to empty strings.
	//
	// (1) If both directoryName and fileName are non-empty, the following directory structure is assumed:
	// 	
	// --TFile
	// ----TDirectoryFile
	// ------TObject
	//
	// (2) If directoryName is empty but fileName if non-empty, the following directory structure is assumed:
	//
	// --TFile
	// ----TObject
	//
	// (3) If fileName are empty, the following directory structure is assumed (even if directoryName is non-empty):
	//
	// --TObject
	//
	// For (3), it is assumed the object can be opened directly from a file via an Open() method (e.g. a TASImage or a TFile),
	// 		and that objName contains the path to the object file, rather than just the name.


	// Determine the mode
	bool has_directory = true;
	if(directoryName.empty())
		has_directory = false;

	bool has_file = true;
	if(fileName.empty())
		has_file = false;


	// Retrieve the object in the proper manner
	unique_ptr<TFile> file;

	if(has_file)
	{
		file.reset ( new TFile(fileName.c_str()) );

		// Check for validity of file
		if(!file->IsOpen())
		{
			cout << "ERROR: File could not be opened. Check the validity of the file name.\n";
			return nullptr;
		}
	}

	unique_ptr<TDirectoryFile> tdf;
	unique_ptr<T> objptr;

	if(has_file && has_directory)
	{
		tdf.reset( (TDirectoryFile*)file->Get(directoryName.c_str()) );

		// Check for validity of directory
		if(tdf)
		{
			objptr.reset( (T*)tdf->Get(objName.c_str()) );
		}
		else
		{
			cout << "ERROR: Directory could not be opened. Check the validity of the directory name.\n";
			return nullptr;
		}
	}
	else if(has_file)
	{
		objptr.reset( (T*)file->Get(objName.c_str()) );
	}
	else
	{
		objptr = doOpen<T>(objName);
	}

	// Check for validity of object
	if(!objptr)
	{
		cout << "ERROR: Object could not be loaded.\n";
		return nullptr;
	}

	objptr = releaseOwnership<T>( move(objptr) );


	return objptr;
}


#endif
