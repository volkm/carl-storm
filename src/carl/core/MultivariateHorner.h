 /**
 * @file	MultivariateHorner.h
 * @author	Lukas Netz
 *
 */

#pragma once
#include <vector>
#include "Variable.h"
#include <set>
#include "MultivariatePolynomial.h"
#include "../interval/Interval.h"
#include "../interval/IntervalEvaluation.h"
 #include "MultivariateHornerSettings.h"

#include "Term.h"

namespace carl{

static std::map<Variable, Interval<double>> mMap = {{ Variable::NO_VARIABLE , Interval<double>(0)}};

template<typename PolynomialType, class strategy >
class MultivariateHorner : public std::enable_shared_from_this<MultivariateHorner<PolynomialType, strategy >> { 

/**
* Datastructure to save Polynomes once they are transformed into a horner scheme:
*
* h = Variable * h_dependent + h_independent || Variable * const_dependent + const_independent || Variable * h_dependent + const_independent
*
*/


private:
	typedef typename PolynomialType::CoeffType CoeffType;

	CoeffType mConst_dependent = static_cast<CoeffType>(0);
	CoeffType mConst_independent = static_cast<CoeffType>(0);
	Variable mVariable = Variable::NO_VARIABLE;
	unsigned mExponent = 1;
	std::shared_ptr<MultivariateHorner> mH_dependent;
	std::shared_ptr<MultivariateHorner> mH_independent;

	
public:
		
	//Constuctor
	MultivariateHorner (const PolynomialType&& inPut);
	MultivariateHorner (const PolynomialType&& inPut, std::map<Variable, Interval<double>>& map);
	MultivariateHorner (const PolynomialType&& inPut, bool dummy, std::map<Variable, Interval<double>>& map);


	//~MultivariateHorner ();
	
	MultivariateHorner& operator=(const MultivariateHorner& mh)
	{
		mConst_dependent = mh.mConst_dependent;
		mConst_independent = mh.mConst_independent;

	 	mH_dependent = mh.mH_dependent;
	 	mH_independent = mh.mH_independent;	

	 	mVariable = mh.mVariable;
	 	mExponent = mh.mExponent;		
		return *this;
	}

/*
	MultivariateHorner& operator=(MultivariateHorner&& mh) 
	{
		mConst_dependent = std::move(mh.mConst_dependent);
		mConst_independent = std::move(mh.mConst_independent);	 	
	 
	 	mH_dependent = std::move(mh.mH_dependent);
	 	mH_independent = std::move(mh.mH_independent);		
	 
	 	mVariable = mh.mVariable;		
	 	mExponent = mh.mExponent;
		return *this;
	}
*/

	//getter and setters
	Variable getVariable() const
	{
		return mVariable;
	}

	void setVariable(Variable::Arg& var) 
	{
		mVariable = var;
	}

	std::shared_ptr<MultivariateHorner> getDependent() const
	{
		return mH_dependent;
	}

	void removeDependent()
	{
			mH_dependent = NULL;
	}

	void removeIndepenent() 
	{
			mH_independent = NULL;
	}

	void setDependent(std::shared_ptr <MultivariateHorner> dependent)
	{

		mH_dependent = dependent;
	}

	std::shared_ptr<MultivariateHorner> getIndependent() const
	{
		return mH_independent;
	}

	void setIndependent(std::shared_ptr <MultivariateHorner> independent)
	{
		mH_independent = independent;
	}

	const CoeffType& getDepConstant() const
	{
		return mConst_dependent;
	}

	void setDepConstant(const CoeffType& constant)
	{
		mConst_dependent = constant;
	}	

	const CoeffType& getIndepConstant() const
	{
		return mConst_independent;
	}

	void setIndepConstant(const CoeffType& constant){
		mConst_independent = constant;
	}

	unsigned getExponent() const
	{
		return mExponent;
	}

	void setExponent(const unsigned& exp){
		mExponent = exp;
	}

}; //Class MultivarHorner

}//namespace carl
#include "MultivariateHorner.tpp"


