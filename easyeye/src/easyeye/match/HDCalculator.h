/*********************************************************//**
** @file GetHammingDistance.h
** Methods for calculating the Hamming Distance (HD).
**
** @date 12/2010
** @author: Added and Modified by Yooyoung Lee
**         - Libor Masek (masekl01@csse.uwa.edu.au) - Matlab code
**         - Xiaomei Liu (xliu5@cse.nd.edu) - Translated to C code
**          
** @note Additions and Modifications to existing source code.
**
** Note: Please send BUG reports to yooyoung@<NOSPAM>nist.gov. 
** For more information, refer to: https://cspot-run2.nist.gov/itl/iad/ig/vasir.cfm
**
** @par Disclaimer 
** This software was developed at the National Institute of Standards 
** and Technology (NIST) by employees of the Federal Government in the
** course of their official duties. Pursuant to Title 17 Section 105 
** of the United States Code, this software is not subject to copyright 
** protection and is in the public domain. NIST assumes no responsibility 
** whatsoever for use by other parties of its source code or open source 
** server, and makes no guarantees, expressed or implied, about its quality, 
** reliability, or any other characteristic.
**************************************************************/

#ifndef _HD_CALCULATOR_H_
#define _HD_CALCULATOR_H_

#include "../encode/easyeye_encode.h"

namespace easyeye
{
/**
 * Class for calculating the Hamming Distance between two iris templates.
 *
 * The class incorporates noise masks, i.e. noise bits are not used when 
 * calculating the HD.
 */
class HDCalculator
{
 public:
	HDCalculator();
	virtual ~HDCalculator();

    static const double HD_NAN;
    
	/**
	* Calculates HD shifting left and right and then returning the minimum 
	* HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD Minimum
	*/
	double computeHDX(const Encoding&,
					  const Encoding&,
					  int);
	/**
	* Calculate HD in shifting towards the up and down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD Minimum
	*/
	double computeHDY(const Encoding&,
					   const Encoding&,
					   int); 

	/**
	* Calculate HD in shifting towards left/right or up/down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD minimum
	*/
	double computeHDXorY(const Encoding&,
					   const Encoding&,
					   int); 

	/**
	* Calculate HD in shifting towards left/right and up/down
	* and return minimum HD value.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	* @param scales  Number of filters used for encoding - needed to
    *                 determine how many bits should be moved when shifting.
	* @return HD minimum
	*/
	double computeHDXandY(const Encoding&,
						   const Encoding&,
						   int); 
	
 private:

	int width;
	int height;
	int maxShiftX;
	int maxShiftY;
  
	int *template1s;
	int *mask1s;
	int *template2s;
	int *mask2s;

	int *mask;
	int *C;

	/**
	* Initialize the given templates.
	*
	* @param classTemplate1 Input target template
	* @param classTemplate2 Input query template
	*/
	bool initializeTemplates(const Encoding&,
						       const Encoding&);

	/**
	* Calculates HD.
	*
	* @param tTemplate Input target template
	* @param tMask Input target mask incorperating with noise
	* @param qTemplate Input query template
	* @param qMask Input query mask incorperating with noise
	* @param newTemplate Input new template
	* @param newMask Input new mask incorperating with noise
	* @return HD minimum
	*/

    double calcHD(int* tTemplate, int* tMask, int* qTemplate, int* qMask, 
				int* newTemplate, int* newMask);

   /**
	* Shifts the iris patterns bitwise.
	* In order to provide the best match each shift is by two bits 
	* to the left and to the right.
	*
	* @param templates Input template
	* @param width Input width
	* @param height Input height
	* @param noshifts >0: Number of shifts to the right
	* 				  <0: Number of shifts to the left
	* @param nscales  Number of filters used for encoding, needed to
    *                 determine how many bits should be moved when shifting
	* @param templatenew Used to return the shifted template
	*/
	void X_Shiftbits(int *templates, int width, int height, 
					int noshifts, int nscales, int *templatenew); 
	
	/**
	* Shifts the iris patterns bitwise.
	* In order to provide the best match each shift is one bit up and down.
	*
	* @param templates Input template
	* @param width Input width
	* @param height Input height
	* @param noshifts Number of shifts to the down, a negative
	*                 value in shifting to the up.
	* @param nscales  Number of filters used for encoding, needed to
    *                 determine how many bits should be moved when shifting
	* @param templatenew Used to return the shifted template
	*/
	void Y_Shiftbits(int *templates, int width, int height, 
					int noshifts,int nscales, int *templatenew);
	
}; // HDCalculator

}
#endif // !_HD_CALCULATOR_H_
