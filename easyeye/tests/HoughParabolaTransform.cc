//class HoughParabolaTransform
//{
//public:
//    HoughParabolaTransform(vector<double> a_range__, vector<double> b_range__, vector<double> c_range__)
//        : a_range_(a_range__), b_range_(b_range__), c_range_(c_range__), max_candidates_(1), accumulator(), debug_(true)
//    {
//    }
//    HoughParabolaTransform(const ParamRange<double>& a_range, const ParamRange<double>& b_range, const ParamRange<double>& c_range)
//        : a_range_(a_range.values()), b_range_(b_range.values()), c_range_(c_range.values()), max_candidates_(1), accumulator(), debug_(true)
//    {
//    }
//    
//    virtual bool CanVote(int x, int y, double a, double b, double c, const Mat& image) {
//        return x >= 0 && x < image.cols && y >= 0 && y < image.rows;
//    }
//    
//    float GetPixelValue(const Mat& image, double x, double y) {
//        return GetSubpixel(image, x, y);
//    }
//    
//    Vec3d Compute(const Mat& image, vector<Vec3d> candidates) {
//        int sizes[] = { (int) a_range_.size(), (int) b_range_.size(), (int) c_range_.size() };
//        accumulator.create(3, sizes, CV_32F);
//        accumulator = Scalar(0.0f);
//        if (debug_) {
//            cerr << "a range: ";
//            for (vector<double>::iterator ait = a_range_.begin(); ait != a_range_.end(); ++ait) {
//                cerr << *ait << ' ' ;
//            }
//            cerr << endl;
//            cerr << "b range: ";
//            for (vector<double>::iterator bit = b_range_.begin(); bit != b_range_.end(); ++bit) {
//                cerr << *bit << ' ' ;
//            }
//            cerr << endl;
//            cerr << "c range: ";
//            for (vector<double>::iterator cit = c_range_.begin(); cit != c_range_.end(); ++cit) {
//                cerr << *cit << ' ' ;
//            }
//            cerr << endl;
//        }
//        for (size_t ai = 0; ai < a_range_.size(); ai++) {
//            double a = a_range_[ai];
//            for (size_t bi = 0; bi < b_range_.size(); bi++) {
//                double b = b_range_[bi];
//                for (size_t ci = 0; ci < c_range_.size(); ci++) {
//                    double c = c_range_[ci];
//                    for (int x = 0; x < image.cols; x++) {
//                        double y = a * x * x + b * x + c;
//                        if (IsInsideIntLimits(y)) {
//                            int rx = (int) round(x), ry = (int) round(y);
//                            if (CanVote(rx, ry, a, b, c, image)) {
//                                float pixel = GetPixelValue(image, x, y);
//                                accumulator.at<float>(ai, bi, ci) += pixel;
//                            }
//                        }
//                    }
//                    ci++;
//                }
//                bi++;
//            }
//            ai++;
//        }
//        int max_index[3];
//        double max_accumulated;
//        minMaxIdx(accumulator, NULL, &max_accumulated, NULL, max_index);
//        if (debug_) {
//            cerr << "hough: max accumulation at index " << max_index[0] << ' ' << max_index[1] << ' ' << max_index[2] << " = " << max_accumulated << endl;
//        }
//        Vec3d most_votes(a_range_[max_index[0]], b_range_[max_index[1]], c_range_[max_index[2]]);
//        candidates.push_back(most_votes);
//        return most_votes;
//    }
//private:
//    vector<double> a_range_, b_range_, c_range_;
//    size_t max_candidates_;
//    Mat accumulator;
//    bool debug_;
//};


