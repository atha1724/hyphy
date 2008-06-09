/*
 *  Bgm.h
 *  HYPHY_XCode
 *
 *  Created by Art Poon on 2/5/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "hy_lists.h"
#include "classes.h"
#include "likefunc.h"
#include "parser.h"
#include <math.h>
#include "matrix.h"
#include "baseobj.h"
#include "batchlan.h"
// #include "HYUtils.h"

/* ____________________________________________________________________________________________________________________________________	*/
class Bgm : public _LikelihoodFunction
{
public:
	Bgm (_AssociativeList *, _AssociativeList *);		// constructor
	
	virtual ~Bgm (void);		// destructor
	
	
					
	virtual	_Parameter		Compute (void);			// function polymorphism, no argument returns likelihood of network
													// specified by _Matrix object dag
				
						
							
				
	
	virtual _Matrix *		Optimize ();	// estimate maximum posterior network via greedy hill-climbing heuristic
	
	//virtual _Matrix *		Optimize (_SimpleList *);
					
	virtual _PMathObj		CovarianceMatrix (_SimpleList *);	// wrapper function for MCMC, to explore posterior distribution 
	
	
	
	void			SetDataMatrix (_Matrix *),
					SetWeightMatrix (_Matrix *),
					SetGraphMatrix (_Matrix *),
					SetBanMatrix (_Matrix *),
					SetEnforceMatrix (_Matrix *),
					SetBestOrder (_SimpleList *);
	
	_Matrix *		ExportNodeScores (void);
	_Matrix *		ExportGraph (void);
	
	void			ImportNodeScores (_Matrix *);
	
	
	_Parameter		ComputeDiscreteScore (long node_id),	// compute K2 or BDeu scoring metric for a discrete node in network
															// with discrete-valued parents, given data and prior
															// use BDeu if prior_sample_size > 0.
					ComputeDiscreteScore (long, _Matrix *),
					ComputeDiscreteScore (long, _SimpleList &),
					
					ImputeDiscreteScore (long, _SimpleList &),
					
					ComputeContinuousScore (long node_id),	// compute scoring metric for continuous node with both discrete
															// and continuous parents.
					ComputeContinuousScore (long, _Matrix *);
	
	long			GetNumNodes (void)		{ return num_nodes; }
	long			GetNumCases (void)		{ return (obsData ? obsData->GetHDim() : 0); }
	_SimpleList		GetLastOrder (void)		{ return last_node_order; }
	
protected:
	_Parameter		Compute (_SimpleList*, _List*),			// return likelihood of node ordering, model averaging over all
															// possible networks that are consistent with the order.
					Compute (_Matrix *);
	
	void			CacheNodeScores (void);
	void			ReleaseNodeScores (void);
	//unsigned long	IndexIntoCache (long, _SimpleList);
	//void			IndicesFromCache (long);
	
	void			InitComputeLists (_List *);
	void			DumpComputeLists (_List *);
	
	
	_Parameter		LnGamma (_Constant *, _Parameter),
					LogSumExpo (_GrowingVector *);
	
	_Matrix *		RunColdChain (_SimpleList *, long, long);
	void			RunHotChain (_SimpleList *, long, long, _Parameter);
	
	_Matrix *		GraphMCMC (bool);
	
		/* ------------- these functions are used only for greedy search heuristic ---------------- */
	_Parameter		TryEdge (long, long, long, _Parameter);
	
	void			RandomizeDag (long),
					RandomizeGraph (_Matrix *, _SimpleList *, long, bool),
					PrintGraph (_Matrix *),
					ResetGraph (_Matrix *);
	
	long			MarginalSum (bool, long);
	
	bool			IsCyclic (void);
		/* ----------------------------------------------------------------------------------------	*/
	
private:
	
	long			num_nodes,			// number of variables in network
	
					max_max_parents;	// the global maximum number of parents per node
	
	
	_Constant	*	calc_bit;			// provides access to mathematical functionality of _Constant
	
	
	_Matrix			dag,				// i.e. directed acyclic graph as a square adjacency matrix,
										// where 1 indicates presence of edge (row --> col)
					
	
					banned_edges,		// complements DAG, 1 entry indicates banned edge
					enforced_edges,
	
					prior_sample_size,	// prior distributions for hyperparameters
					prior_mean,
					prior_precision;
					
					
	_SimpleList		is_discrete,		// vector containing 1 for discrete node, 0 for continuous
		
					num_levels,			// vector containing number of levels for discrete nodes
					
					max_parents,		// maximum number of parents allowed per node
					
					has_missing,		// data matrix for i-th node contains missing values
					
					last_node_order,	// store node orderings from order-MCMC
					best_node_order;
					
	_List			node_scores,
					dag_parameters;
	
	
	_Matrix		*	obsData,			// data matrix read from file
		
				*	obsWeights;			// vector of weights corresponding to each observation in data matrix
		
	
	bool			scores_cached,
					is_dynamic_graph;
};



/* ____________________________________________________________________________________________________________________________________	*/
#ifdef __NEVER_DEFINED__
class _DynamicBgm : public Bgm
{
public:
	_DynamicBgm ();		// default constructor does nothing
	_DynamicBgm (_AssociativeList *, _AssociativeList *, _AssociativeList *);
	
	virtual ~_DynamicBgm (void);		// destructor
private:
};


bool		ReadDataFromFile	(char, _Matrix &);
#endif


