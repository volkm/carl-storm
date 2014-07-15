/* 
 * File:   RealAlgebraicNumber.h
 * Author: Gereon Kremer <gereon.kremer@cs.rwth-aachen.de>
 */

#pragma once

#include <memory>

#include "../util/SFINAE.h"
#include "../numbers/numbers.h"
#include "../core/Sign.h"
#include "UnivariatePolynomial.h"

#include "RealAlgebraicNumberSettings.h"

namespace carl {

/**
 * This class is the base for all representations of real algebraic numbers and provides crucial operations such as arithmetic, ordering or sign determination on them.
 */
/// @todo Add `EnableIf<is_fraction<Number>>` such that gcc does not crash.
template<typename Number>
class RealAlgebraicNumber {
protected:
	////////////////
	// Attributes //
	////////////////

	/**
	 * flag indicating whether this number represents a root of a polynomial or an intermediate point
	 */
	bool mIsRoot;
	/**
	 * flag indicating whether this number is representable by a numeric
	 */
	bool mIsNumeric;
	/**
	 * the exact numeric value of this number if available, otherwise mIsNumeric is false and mValue 0
	 */
	Number mValue;

private:
	std::weak_ptr<RealAlgebraicNumber> pThis;
	std::shared_ptr<RealAlgebraicNumber> thisPtr() const {
		return std::shared_ptr<RealAlgebraicNumber>(this->pThis);
	}

protected:
	//////////////////////////
	// Con- and destructors //
	//////////////////////////

	/**
	 * Constructs a real algebraic number with a specified marking as to whether this number stems from a real root computation or not.
	 * @param isRoot true marks this real algebraic number to stem from a root computation
	 * @param isNumeric true marks this real algebraic number to be representable as an exact numeric (standard is false)
	 * @param value the exact numeric value of this number if available, otherwise mIsNumeric is false and mValue 0
	 */
	RealAlgebraicNumber(bool isRoot, bool isNumeric = false, const Number& value = 0) :
			mIsRoot(isRoot),
			mIsNumeric(isNumeric),
			mValue(value)
	{
	}
public:
	/**
	 * Destructor.
	 */
	~RealAlgebraicNumber() {
	}

	static std::shared_ptr<RealAlgebraicNumber> create(bool isRoot, bool isNumeric = false, const Number& value = 0) {
		auto res = std::shared_ptr<RealAlgebraicNumber>(new RealAlgebraicNumber(isRoot, isNumeric, value));
		res->pThis = res;
		return res;
	}

	virtual std::shared_ptr<RealAlgebraicNumber<Number>> clone() const {
		return RealAlgebraicNumber<Number>::create(isRoot(), isNumeric(), value());
	}

	///////////////
	// Selectors //
	///////////////
	/**
	 * @return the flag marking whether the real algebraic number stems from a root computation or not
	 */
	bool isRoot() const {
		return mIsRoot;
	}

	/**
	 * Set the flag marking whether the real algebraic number stems from a root computation or not.
	 * @param isRoot
	 */
	void setIsRoot(bool isRoot) {
		mIsRoot = isRoot;
	}

	/**
	 * Returns true if the number is exactly zero.
	 * @return true if an exact numeric representation was found during the refinements, false otherwise.
	 */
	virtual bool isZero() const {
		return this->value() == 0;
	}

	/**
	 * Returns true if an exact numeric representation was found during the refinements.
	 * 
	 * If the return value is false, this guarantees that this object is an instance of RealAlgebraicNumberIR.
	 * @return true if an exact numeric representation was found during the refinements, false otherwise.
	 */
	bool isNumeric() const {
		return mIsNumeric;
	}

	/**
	 * Returns true if the number is represented numericly, that means not as an interval.
	 * 
	 * @return 
	 */
	virtual bool isNumericRepresentation() const {
		return true;
	}

	/**
	 * Returns a point that is suitable for splitting during branch and bound when searching for integer solutions.
	 * If this number is numeric, it returns the same as value().
	 * If this number is represented by an interval, it returns some value from this interval that is not integral.
	 * Thus, if the result of this method is integral, an integral value has been found and otherwise, branching at the returned point guarantees progress.
	 * @return a suitable branching point.
	 */
	virtual Number branchingPoint() const {
		return this->value();
	}

	/** Gives an exact numeric representation of this real algebraic number which could have been found during the refinement steps.
	 * The method returns 0 if the value was never set during refinement.
	 * @return an exact numeric representation of this real algebraic number which could have been found during the refinement steps
	 */
	const Number& value() const {
		return mValue;
	}
	
	///////////////
	// Operators //
	///////////////

	// assignment operators

	/**
	 * This real algebraic number gets all values of the other.
	 * @param o other real algebraic number
	 */
	virtual const RealAlgebraicNumber<Number>& operator=(const RealAlgebraicNumber<Number>& o) {
		mIsRoot = o.mIsRoot;
		mValue = o.mValue;
		mIsNumeric = o.mIsNumeric;
		return *this;
	}

	virtual bool containedIn(const Interval<Number>& i) const {
		return i.contains(this->value());
	}

	////////////////
	// Operations //
	////////////////

	/**
	 * Returns sign (GiNaC::ZERO_SIGN, GiNaC::POSITIVE_SIGN, GiNaC::NEGATIVE_SIGN) of this real algebraic number.
	 * @return sign of this real algebraic number.
	 */
	virtual Sign sgn() const {
		if (this->value() == 0) return Sign::ZERO;
		else if (this->value() > 0) return Sign::POSITIVE;
		else return Sign::NEGATIVE;
	}

	/**
	 * Returns sign (GiNaC::ZERO_SIGN, GiNaC::POSITIVE_SIGN, GiNaC::NEGATIVE_SIGN) of the specified univariate polynomial at this real algebraic number.
	 * @param p rational univariate polynomial
	 * @return sign of the univariate polynomial at this real algebraic number.
	 */
	virtual Sign sgn(const UnivariatePolynomial<Number>& p) const {
		Number x = p.evaluate(this->value());
		if (x == 0) return Sign::ZERO;
		else if (x > 0) return Sign::POSITIVE;
		else return Sign::NEGATIVE;
	}

	/** Computes a numeric value for this real algebraic number approximating it.
	 * @complexity constant
	 * @return a numeric value for this real algebraic number approximating it
	 */
	virtual const Number approximateValue() const {
		return this->value();
	}
	
	template<typename Num>
	friend std::ostream& operator<<(std::ostream& os, const RealAlgebraicNumber<Num>* g);
};

template<typename Number>
using RealAlgebraicNumberPtr = std::shared_ptr<RealAlgebraicNumber<Number>>;
}

#include "RealAlgebraicNumberIR.h"
#include "RealAlgebraicNumberNR.h"
#include "RealAlgebraicNumberOperations.h"

namespace carl {

template<typename Number>
std::ostream& operator<<(std::ostream& os, const carl::RealAlgebraicNumber<Number>* g) {
	if (g == nullptr) return os << "nullptr";
	if (g->isNumericRepresentation()) {
		return os << static_cast<const carl::RealAlgebraicNumberNR<Number>*>(g);
	} else {
		return os << static_cast<const carl::RealAlgebraicNumberIR<Number>*>(g);
	}
}

}