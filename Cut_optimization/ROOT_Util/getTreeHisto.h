//
// getTreeHisto.h
// Utilities/
// 
// Created by Johann Gan on 6/2/17
// 
// 
// Template function. Retrieves a histogram (with specified attributes) for a specified branch in a tree object.
//
// class H is the histogram type.
// class T is the tree type (can be inferred). Works with TChains, TTrees, TNtuples.

//////////////////////////////////////

#ifndef GETTREEHISTO_H_
#define GETTREEHISTO_H_

//////////////////////////////////////

// Dependencies
#include <string>
#include <memory>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TChain.h>

//////////////////////////////////////

using namespace std;


// 1D histogram
template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, const string& branch, const string& cuts, \
	const string& name, const string& title, int nbinsx, double xlo, double xhi)
{
	unique_ptr<H> histo ( new H(name.c_str(), title.c_str(), nbinsx, xlo, xhi) );
	tree->Draw( (branch + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}

template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, const string& branch, const string& cuts, \
	const string& name, const string& title, int nbinsx, double xlowedges[])
{
	unique_ptr<H> histo ( new H(name.c_str(), title.c_str(), nbinsx, xlowedges) );
	tree->Draw( (branch + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}



// 2D histogram
template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, const string& branchx, const string& branchy, const string& cuts, \
	const string& name, const string& title, int nbinsx, double xlo, double xhi, int nbinsy, double ylo, double yhi)
{
	unique_ptr<H> histo( new H(name.c_str(), title.c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi) );
	tree->Draw( (branchy + ":" + branchx + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}

template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, const string& branchx, const string& branchy, const string& cuts, \
	const string& name, const string& title, int nbinsx, double xlowedges[], int nbinsy, double ylowedges[])
{
	unique_ptr<H> histo( new H(name.c_str(), title.c_str(), nbinsx, xlowedges, nbinsy, ylowedges) );
	tree->Draw( (branchx + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}


// 3D histogram
template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, \
	const string& branchx, const string& branchy, const string& branchz, const string& cuts, \
	const string& name, const string& title, \
	int nbinsx, double xlo, double xhi, int nbinsy, double ylo, double yhi, int nbinsz, double zlo, double zhi)
{
	unique_ptr<H> histo( new H(name.c_str(), title.c_str(), nbinsx, xlo, xhi, nbinsy, ylo, yhi, nbinsz, zlo, zhi) );
	tree->Draw( (branchx + ":" + branchy + ":" + branchz + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}

template <class H, class T>
unique_ptr<H> getTreeHisto(const unique_ptr<T>& tree, \
	const string& branchx, const string& branchy, const string& branchz, const string& cuts, \
	const string& name, const string& title, \
	int nbinsx, double xlowedges[], int nbinsy, double ylowedges[], int nbinsz, double zlowedges[])
{
	unique_ptr<H> histo( new H(name.c_str(), title.c_str(), nbinsx, xlowedges, nbinsy, ylowedges, nbinsz, zlowedges) );
	tree->Draw( (branchx + ":" + branchy + ":" + branchz + ">>" + name).c_str(), cuts.c_str(), "goff" );

	return histo;
}



#endif