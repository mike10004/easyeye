#pragma once

#if 0
#if defined MASEK_EXPORTS
#define MASEK_API __declspec(dllexport)
#else
#define MASEK_API __declspec(dllimport)
#endif
#endif


#include "global.h"

/**
 * This class gives all Masek methods a unified namespace, and makes it easy
 * to overload the methods, i.e. to change the internal logic.
 *
 * All methods unless specified otherwise were original written by
 * Libor Masek, and ported to C by Xiaomei Liu.
 *
 * @author Yooyoung Lee
 */
//class MASEK_API Masek 
class Masek 
{
public:
  /** \defgroup GlobalH Global Types and Constants found in global.h */
  /*@{*/
  //static const double PI;
  //static const double AdjPrecision;
  #define PI (double)3.14159265358979
  #define AdjPrecision (double)0.0000000000005

  typedef struct mat_data   MAT_DATA;
  typedef struct _image     IMAGE;
  typedef struct _filter    filter;
  typedef struct a_complex  Complex;

  /*@}*/

  /**
   * Adjusts image gamma.
   *
   * @author Peter Kovesi
   *  
   * @param im  image to be processed
   * @param g   image gamma value.\n
   *            Values in the range 0-1 enhance contrast of bright regions, 
   *            values > 1 enhance contrast in dark regions. 
   * @return    the modified filter
   */
  filter* adjgamma(filter* im, double g);

  /**
   * Canny edge detection
   *
   * Function to perform Canny edge detection. Code uses modifications as
   * suggested by Fleck (IEEE PAMI No. 3, Vol. 14. March 1992. pp 337-345)
   *
   * @author Peter Kovesi
   * @author Libor Masek
   *
   * @param im        image to be procesed
   * @param sigma     standard deviation of Gaussian smoothing filter 
   *                  (typically 1)
   * @param scaling   factor to reduce input image by
   * @param vert      weighting for vertical gradients
   * @param horz      weighting for horizontal gradients
   * @param gradient  (OUT) edge strength image (gradient amplitude)
   * @param orND      (OUT) orientation image (in degrees 0-180, positive 
   *                  anti-clockwise)
   * @return          the modified image
   *
   * @see nonmaxsup, hysthresh
   */
  IMAGE* canny(IMAGE* im, double sigma, double scaling, double vert,
                       double horz, filter* gradient, filter* orND);

  /**
   * Returns the pixel coordinates of a circle defined by the radius and 
   * x, y coordinates of its centre.
   *
   * @param x0      centre coordinates of the circle (x)
   * @param y0      centre coordinates of the circle (y) 
   * @param r       the radius of the circle
   * @param imgsize size of the image array to plot coordinates onto
   * @param _nsides the circle is actually approximated by a polygon, this
   *                argument gives the number of sides used in this 
   *                approximation. Default is 600.
   * @param x       (OUT) an array containing x coordinates of circle 
   *                boundary points
   * @param y       (OUT) an array containing y coordinates of circle 
   *                boundary points
   */
  int circlecoords(double x0, double y0, double r, int* imgsize, 
                           double _nsides, int** x, int** y);

  /**
   * Generates a biometric template from an iris in an eye image.
   *
   * Arguments:
   * @param eyeimage_filename the file name of the eye image
   * @param _template         (OUT) the binary iris biometric template
   * @param mask              (OUT) the binary iris noise mask
   */
//  void createiristemplate(char* eyeimage_filename, int nscales, 
//                                  int** _template, int** mask, 
//                                  int* width, int* height);
  
  /**
   * Generates a biometric template from the normalised iris region, also 
   * generates corresponding noise mask
   *
   * @param polar_array   normalised iris region
   * @param noise_array   corresponding normalised noise region map
   * @param nscales       number of filters to use in encoding
   * @param minWaveLength base wavelength
   * @param mult          multicative factor between each filter
   * @param sigmaOnf      bandwidth parameter
   * @param _template     (OUT) the binary iris biometric template
   * @param mask          (OUT) the binary iris noise mask
   */
  void encode(filter* polar_array, IMAGE* noise_array, int nscales, 
                      int minWaveLength, int mult, double sigmaOnf, 
                      int* irisTemplate, int* irisMask);
//                      int** _template, int** mask, int* width, int* height);

  /**
   * Returns the coordinates of a circle in an image using the Hough 
   * transform and Canny edge detection to create the edge map.
   * 
   * The output depends on the input:
   * - \b circleiris centre coordinates and radius of the detected iris 
   *      boundary
   * - \b circlepupil centre coordinates and radius of the detected pupil 
   *      boundary
   * - \b imagewithnoise original eye image, but with location of noise 
   *      marked with NaN values
   *
   * @param image     the image in which to find circles
   * @param lradius   lower radius to search for
   * @param uradius   upper radius to search for
   * @param scaling   scaling factor for speeding up the Hough transform
   * @param sigma     amount of Gaussian smoothing to apply for creating 
   *                  edge map.
   * @param hithres   threshold for creating edge map
   * @param lowthres  threshold for connected edges
   * @param vert      vertical edge contribution (0-1)
   * @param horz      horizontal edge contribution (0-1)
   * @param _row      (OUT) center x-coordinate
   * @param _col      (OUT) center x-coordinate
   * @param _r        (OUT) radius
   */
  void findcircle(IMAGE* image, int lradius, int uradius, 
                          double scaling, double sigma, double hithres, 
                          double lowthres, double vert, double horz, 
                          int* _row, int* _col, int* _r);

  /**
   * Returns the coordinates of a line in an image using the linear Hough 
   * transform and Canny edge detection to create the edge map.
   * 
   * @param image the input image
   * @param lines (OUT) parameters of the detected line in polar form.\n
   *              0=cos(t), 1=sin(t), 2=r
   * @return      number of lines
   */
  int findline(unsigned char* data, int rows, int cols, double* *lines);
    int findline(IMAGE* image, double **lines);
  /**
   * Function for convolving each row of an image with 1D log-Gabor filters
   *
   * @author Peter Kovesi
   * @author Libor Masek
   *
   * @param im            the image to convolve
   * @param nscale        number of filters to use
   * @param minWaveLength wavelength of the basis filter
   * @param mult          multiplicative factor between each filter
   * @param sigmaOnf      ratio of the standard deviation of the Gaussian 
   *                      describing the log Gabor filter's transfer 
   *                      function in the frequency domain to the filter 
   *                      center frequency.
   * @param EO            (OUT) an 1D cell array of complex valued 
   *                      comvolution results
   */
  void gaborconvolve(filter* im, int nscale, int minWaveLength, 
                             int mult, double sigmaOnf, Complex*** EO, 
                             double** filtersum, int* EOh, int* EOw);

  /**
   * Takes an edge map image, and performs the Hough transform for finding 
   * circles in the image.
   *
   * @param m_edgeim  the edge map image to be transformed
   * @param rmin      the minimum radius values of circles to search for
   * @param rmax      the maximum radius values of circles to search for
   * @return          the Hough transform
   */
  double* houghcircle(filter* m_edgeim, int rmin, int rmax);

  /**
   * Hysteresis thresholding
   *
   * Function performs hysteresis thresholding of an image.
   * All pixels with values above threshold T1 are marked as edges
   * All pixels that are adjacent to points that have been marked as edges
   * and with values above threshold T2 are also marked as edges. Eight
   * connectivity is used.
   *
   * @author Peter Kovesi 
   *
   * @param im  image to be thresholded (assumed to be non-negative)
   * @param T1  upper threshold value
   * @param T2  lower threshold value
   * @return    the thresholded image (containing values 0 or 1)
   */
  filter* hysthresh(filter* im, double T1, double T2);

  /**
   * INTERP2 2-D interpolation (table lookup).
   * Linear interpolation.
   *
   * ZI = INTERP2(Z,XI,YI) interpolates to find ZI, the values of the
   * underlying 2-D function Z at the points in matrices XI and YI.
   *
   * @author MathWorks, Inc.
   * 
   * @param z  underlying z-function
   * @param xi row vector (matrix w/ constant columns)
   * @param yi column vector (matrix w/ constant rows)
   * @param zi (OUT) created matrix
   */
  void interp2(const filter* z, const filter* xi, const filter* yi, const filter* zi);

  /**
   * Returns the x y coordinates of positions along a line.
   *
   * @param lines an array containing parameters of the line in form
   * @param row   height of the image, needed so that x y coordinates are 
   *              within the image boundary
   * @param cols  width of the image
   * @param x     (OUT) x-coordinates
   * @param y     (OUT) y-coordinates 
   */
  void linescoords(double* lines, int row, int col, int* x, int* y);

  /**
   * Function for performing non-maxima suppression on an image using an
   * orientation image.  It is assumed that the orientation image gives 
   * feature normal orientation angles in degrees (0-180).
   *
   * Note: This function is slow (1 - 2 mins to process a 256x256 image).
   * 
   * @author Peter Kovesi
   * 
   * @param inimage image to be non-maxima suppressed.
   * @param orient  image containing feature normal orientation angles in 
   *                degrees (0-180), angles positive anti-clockwise.
   * @param radius  distance in pixel units to be looked at on each side of 
   *                each pixel when determining whether it is a local maxima 
   *                or not.\n
   *                (Suggested value about 1.2 - 1.5)
   */
  filter* nonmaxsup(filter* inimage, filter* orient, double radius);

  /**
   * Performs normalisation of the iris region by unwraping the circular 
   * region into a rectangular block of constant dimensions.
   *
   *
   * @param image             the input eye image to extract iris data from
   * @param xiris             the x coordinate of the circle defining the 
   *                          iris boundary
   * @param yiris             the y coordinate of the circle defining the 
   *                          iris boundary
   * @param riris             the radius of the circle defining the iris
   *                          boundary
   * @param xpupil            the x coordinate of the circle defining the 
   *                          pupil boundary
   * @param ypupil            the y coordinate of the circle defining the 
   *                          pupilboundary
   * @param rpupil            the radius of the circle defining the pupil
   *                          boundary
   * @param eyeimage_filename original filename of the input eye image
   * @param radpixels         radial resolution, defines vertical dimension 
   *                          of normalised representation
   * @param angulardiv        angular resolution, defines horizontal 
   *                          dimension of normalised representation
   * @param polar_array       (OUT)
   * @param polar_noise       (OUT)
   */
  void normaliseiris(filter* image, int xiris, int yiris, int riris, 
                             int xpupil, int ypupil, int rpupil, 

                             int radpixels,
                             int angulardiv, const filter* polar_array, 
                             const IMAGE* polar_noise);

  /**
   * Radon transform.
   *
   * Evaluates the Radon transform P, of I along the angles specified by the 
   * vector THETA. THETA values measure angle counter-clockwise from the 
   * horizontal axis. THETA defaults to [0:179] if not specified.
   *
   * @author MathWorks, Inc.
   *
   * @param I         image
   * @param THETA     Thetas (must contain \c numangles elements)
   * @param imgrow    image height
   * @param imgcol    image width
   * @param numangles Number of angles
   * @param P         (OUT) Transformation
   * @param R         (OUT) Is a vector giving the values of r corresponding to 
   *                  the rows of P.
   * @return          number of elements in \c R
   */
  int radonc(double* I, double* THETA, int imgrow, int imgcol, 
                     int numangles, double** P, double** R);

  /**
   * Saves a biometric template.
   *
   * @param filedir             directory containing the image
   * @param _eyeimage_filename  the file name of the eye image
   * @param templatedir         directory where the template should be stored
   * @param segdir              directory containing the segmentation data
   * @param nscales             number of filters to use in encoding
   * @param _template           the binary iris biometric template
   * @param _mask               the binary iris noise mask
   * @param mode                \c 1=\c, \c 2= \c segmentiris_gt, 
   *                            \c 3 = \c segmentiris_iridian
   * @param eyelidon            search eyelid
   * @param highlighton         highlight eyelashes/ eyelid (yes=1)
   * @param highlightvalue      value to be used for highlighting the eyelashes
   */
//  void saveiristemplate(const char* filedir, 
//                                const char* templatedir, const char *segdir, 
//                                const char* _eyeimage_filename, int nscales, 
//                                int** _template, int** mask, int* width, 
//                                int* height, int mode, int eyelidon, 
//                                int highlighton, int highlightvalue);

  /**
   * Peforms automatic segmentation of the iris region from an eye image. Also 
   * isolates noise areas such as occluding eyelids and eyelashes.
   *
   * Usage: 
   * [circleiris, circlepupil, imagewithnoise] = segmentiris(image)
   *
   * @param eyeimage        the input eye image
   * @param circleiris      (OUT) centre coordinates and radius of the detected 
   *                        iris boundary
   * @param circlepupil     (OUT) centre coordinates and radius of the detected 
   *                        pupil boundary
   * @param imagewithnoise  (OUT) original eye image, but with location of noise
   *                        marked with NaN values
   * @param eyelidon        search eyelid
   * @param highlighton     highlight eyelashes/ eyelid (yes=1)
   * @param highlightvalue  value to be used for highlighting the eyelashes
   */
  void segmentiris(IMAGE* eyeimage, int* circleiris, 
                           int* circlepupil, double* imagewithnoise, 
                           int eyelidon, int highlighton, 
                           int highlightvalue);
   
  /**
   * Function to shift the bit-wise iris patterns in order to provide the best 
   * match each shift is by two bit values and left to right, since one pixel 
   * value in the normalised iris pattern gives two bit values in the template
   * also takes into account the number of scales used.
   *
   * @param template    the template to shift
   * @param width       width of the template
   * @param height      height of the template 
   * @param noshifts    number of shifts to perform to the right, a negative
   *                    value results in shifting to the left
   * @param nscales     number of filters used for encoding, needed to determine 
   *                    how many bits to move in a shift
   * @param templatene  (OUT) the shifted template
   */
  void shiftbits(int* templates, int width, int height, 
                         int noshifts,int nscales, int* templatenew);

//  double gethammingdistance(int *template1, int *mask1, 
//    int *template2, int *mask2, int scales, int width, int height);

  /** \defgroup GlobalCPP global.cpp */
  /*@{*/
//  void printfilter(filter* mfilter, char* filename);
//  void printimage(IMAGE* m, char* filename);
  /*@}*/

  /** \defgroup MyMatCPP mymat.cpp */
  /*@{*/
//  int loadtemplate(const char *file, MAT_DATA** m_data);
//  int savetemplate(const char *file, MAT_DATA* m_data);
//  int loadsegmentation(const char *file, MAT_DATA** m_data);
//  int savesegmentation(const char *file, MAT_DATA* m_data);
  /*@}*/

  //LEE:
  //int roundND(double);

  //LEE
  /** imread.cpp */
  //IMAGE* imread(const char *name);
  /** imwrite.cpp */
  //void imwrite (const char* filename, IMAGE* image);

protected:
  /** \defgroup GaborconvolveCPP gaborconvolve.cpp */
  /*@{*/
  /** Shift zero-frequency component to center of spectrum.
   * For vectors, FFTSHIFT(X) swaps the left and right halves of X.  For 
   * matrices, FFTSHIFT(X) swaps the first and third quadrants and the 
   * second and fourth quadrants.  For N-D arrays, FFTSHIFT(X) swaps 
   * "half-spaces" of X along each dimension.
   * \c fftshift(x,dim) applies the FFTSHIFT operation along the dimension
   * DIM.
   *
   * @author MathWorks, Inc.
   */
  void fftshift(double *x, int numDims, int size[]);
  void dft(Complex* x, Complex* y, int N);
  Complex* fft(Complex* x, int N);
  /**
   * Compute the inverse FFT of \c x[], assuming its length is a power of 2.
   */
  Complex* ifft(Masek::Complex* x, int N);
  /*@}*/

  /** \defgroup GaussCPP gauss.cpp */
  /*@{*/
  int CREATEGAUSS(int size[2], double sigma, filter *out);
  filter* filter2(filter gaussian, IMAGE *im);
  filter* imresize(filter *im, double scaling);
  /*@}*/

  /** \defgroup GlobalH global.h */
  /*@{*/
  int roundND(double);
  int fix(double);
  /*@}*/

  /** \defgroup SegmentIrisCPP segmentiris.cpp */
  /*@{*/
#if 1
  int segmentiris_gt(IMAGE* eyeimage, char* gndfilename, 
                             int* circleiris, int* circlepupil, 
                             double* imagewithnoise, int eyelidon,
                             int highlighton, int highlightvalue);
  int segmentiris_iridian(IMAGE* eyeimage, char* gndfilename, 
                                  int* circleiris, int* circlepupil, 
                                  double *imagewithnoise, int eyelidon, 
                                  int highlighton, int highlightvalue);
  
#endif
 /*@}*/
};
