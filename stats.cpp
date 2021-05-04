#include "stats.h"

stats::stats(PNG & im){

// 'r' - r
// 'g' - g
// 'b' - b

/* Your code here!! */
/**
   * Initialize the private vectors so that, for each color, entry 
   * (x,y) is the sum of that color value in the rectangle from
   * (0,0) to (x,y). Similarly, for each color, the sumSq vector
   * entry (x,y) is the sum of that color squared over all image
   * pixels from (0,0) to (x,y).
   */
  vector<vector<long>> empty(im.height(), vector<long>(im.width(), 0));
  sumRed = empty;
  sumGreen = empty;
  sumBlue = empty;
  unsigned int w = im.width();
  unsigned int h = im.height();

  for(unsigned int j = 0; j < h; j++){
      if(j==0){
        vector<long> rowRed, rowGreen, rowBlue;
                            // current value alone
        rowRed.push_back((long)(im.getPixel(0,j))->r);
        rowGreen.push_back((long)(im.getPixel(0,j))->g);
        rowBlue.push_back((long)(im.getPixel(0,j))->b);
        for(unsigned int i = 1; i < w; i++){
          RGBAPixel* pix = im.getPixel(i,j);
                            // leftmost + current
          rowRed.push_back(rowRed[i-1]+(long)pix->r);
          rowGreen.push_back(rowGreen[i-1]+(long)pix->g);
          rowBlue.push_back(rowBlue[i-1]+(long)pix->b);
        }
        sumRed[j]=rowRed;
        sumGreen[j]=rowGreen;
        sumBlue[j]=rowBlue;
      }else{
        vector<long> rowRed, rowGreen, rowBlue;
        RGBAPixel* pix = im.getPixel(0,j);
        rowRed.push_back(sumRed[j-1][0]+(long)pix->r);
        rowGreen.push_back(sumGreen[j-1][0]+(long)pix->g);
        rowBlue.push_back(sumBlue[j-1][0]+(long)pix->b);
         
        for(unsigned int i = 1; i < w; i++){
                            // leftmost + current
          RGBAPixel* pix2 = im.getPixel(i,j);
          rowRed.push_back(sumRed[j-1][i]+rowRed[i-1]-sumRed[j-1][i-1]+(long)pix2->r);
          rowGreen.push_back(sumGreen[j-1][i]+rowGreen[i-1]-sumGreen[j-1][i-1]+(long)pix2->g);
          rowBlue.push_back(sumBlue[j-1][i]+rowBlue[i-1]-sumBlue[j-1][i-1]+(long)pix2->b);
        }
        sumRed[j]=rowRed;
        sumGreen[j]=rowGreen;
        sumBlue[j]=rowBlue;
      }      
    }
    std::copy(sumRed.begin(), sumRed.end(), back_inserter(sumsqRed));
    std::copy(sumGreen.begin(), sumGreen.end(), back_inserter(sumsqGreen));
    std::copy(sumBlue.begin(), sumBlue.end(), back_inserter(sumsqBlue));
    //square it
    // for(unsigned int j = 0; j < im.height(); j++){
    //     for(unsigned int i = 0; i < im.width(); i++){
    //         RGBAPixel* p = im.getPixel(i,j);
    //         sumsqRed[j][i] = pow((p->r),2);
    //         sumsqGreen[j][i] =  pow((p->g),2);
    //         sumsqBlue[j][i] = pow((p->b),2);
    //     }
    // } 

    // for(unsigned int j = 0; j < im.height(); j++){
    //     for(unsigned int i = 0; i < im.width(); i++){
    //       std::cout<<sumRed[j][i]<<"     ";
    //     }
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;std::cout<<std::endl;
    // for(unsigned int j = 0; j < im.height(); j++){
    //     for(unsigned int i = 0; i < im.width(); i++){
    //       std::cout<<sumsqRed[j][i]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    for(unsigned int j = 0; j < h; j++){
      if(j==0){
        vector<long> rowRed, rowGreen, rowBlue;
                            // current value alone
        rowRed.push_back(pow((long)(im.getPixel(0,j))->r,2));
        rowGreen.push_back(pow((long)(im.getPixel(0,j))->g,2));
        rowBlue.push_back(pow((long)(im.getPixel(0,j))->b,2));
        for(unsigned int i = 1; i < w; i++){
          RGBAPixel* pix = im.getPixel(i,j);
                            // leftmost + current
          rowRed.push_back(rowRed[i-1]+pow((long)pix->r,2));
          rowGreen.push_back(rowGreen[i-1]+pow((long)pix->g,2));
          rowBlue.push_back(rowBlue[i-1]+pow((long)pix->b,2));
        }
        sumsqRed[j]=rowRed;
        sumsqGreen[j]=rowGreen;
        sumsqBlue[j]=rowBlue;
      }else{
        vector<long> rowRed, rowGreen, rowBlue;
        RGBAPixel* pix = im.getPixel(0,j);
        rowRed.push_back(sumsqRed[j-1][0]+pow((long)pix->r,2));
        rowGreen.push_back(sumsqGreen[j-1][0]+pow((long)pix->g,2));
        rowBlue.push_back(sumsqBlue[j-1][0]+pow((long)pix->b,2));
         
        for(unsigned int i = 1; i < w; i++){
                            // leftmost + current
          RGBAPixel* pix2 = im.getPixel(i,j);
          rowRed.push_back(sumsqRed[j-1][i]+rowRed[i-1]-sumsqRed[j-1][i-1]+pow((long)pix2->r,2));
          rowGreen.push_back(sumsqGreen[j-1][i]+rowGreen[i-1]-sumsqGreen[j-1][i-1]+pow((long)pix2->g,2));
          rowBlue.push_back(sumsqBlue[j-1][i]+rowBlue[i-1]-sumsqBlue[j-1][i-1]+pow((long)pix2->b,2));
        }
        sumsqRed[j]=rowRed;
        sumsqGreen[j]=rowGreen;
        sumsqBlue[j]=rowBlue;
      }      
    }
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
 /**
   * Returns the sums of all pixel values in one color channel.
   * Useful in computing the average color of a rectangle.
   * @param channel is one of r, g, or b
   * @param ul is (x,y) of the upper left corner of the rectangle
   * @param w,h are the width and height of the rectangle
   */
    long sum = 0, leftr=0, leftg=0, leftb=0, upr=0, upg=0, upb=0, lur=0, lug=0, lub=0;
    
    int wnew = ul.first+w;
    int hnew = ul.second+h;
    // for(int j = 0; j < hnew; j++){
    //   for(int i = 0; i < wnew; i++){
    //     std::cout<<sumRed[j][i]<<"     ";
    //   }
    //   std::cout<<std::endl;
    // }
    // std::cout<<std::endl;
    if(ul.first > 0){ 
      leftr = sumRed[hnew-1][(ul.first)-1];
      leftg = sumGreen[hnew-1][(ul.first)-1];
      leftb = sumBlue[hnew-1][(ul.first)-1];
    }
    if(ul.second > 0){
      upr = sumRed[(ul.second)-1][wnew-1];
      upg = sumGreen[(ul.second)-1][wnew-1];
      upb = sumBlue[(ul.second)-1][wnew-1];
    }
    if(ul.first > 0 && ul.second > 0){
      //works
      lur = sumRed[(ul.second)-1][(ul.first)-1];
      lug = sumGreen[(ul.second)-1][(ul.first)-1];
      lub = sumBlue[(ul.second)-1][(ul.first)-1];
    }
    long cr = sumRed[hnew-1][wnew-1];
    long cg = sumGreen[hnew-1][wnew-1];
    long cb = sumBlue[hnew-1][wnew-1];
    
    switch(channel){
    case 'r': //r
        sum = cr-(leftr+upr)+lur;
        break;
    case 'g': //g
        sum = cg-(leftg+upg)+lug;
        break;
    case 'b': //b
        sum = cb-(leftb+upb)+lub;
        break;
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
    long sum = 0,leftr=0, leftg=0, leftb=0, upr=0, upg=0, upb=0, lur=0, lug=0, lub=0;
    int wnew = ul.first+w;
    int hnew = ul.second+h;
    if(ul.first > 0){ 
      leftr = sumsqRed[hnew-1][(ul.first)-1];
      leftg = sumsqGreen[hnew-1][(ul.first)-1];
      leftb = sumsqBlue[hnew-1][(ul.first)-1];
    }
    if(ul.second > 0){
      upr = sumsqRed[(ul.second)-1][wnew-1];
      upg = sumsqGreen[(ul.second)-1][wnew-1];
      upb = sumsqBlue[(ul.second)-1][wnew-1];
    }
    if(ul.first > 0 && ul.second > 0){
      //works
      lur = sumsqRed[(ul.second)-1][(ul.first)-1];
      lug = sumsqGreen[(ul.second)-1][(ul.first)-1];
      lub = sumsqBlue[(ul.second)-1][(ul.first)-1];
    }
    long cr = sumsqRed[hnew-1][wnew-1];
    long cg = sumsqGreen[hnew-1][wnew-1];
    long cb = sumsqBlue[hnew-1][wnew-1];
    
    switch(channel){
    case 'r': //r
        sum = cr-(leftr+upr)+lur;
        break;
    case 'g': //g
        sum = cg-(leftg+upg)+lug;
        break;
    case 'b': //b
        sum = cb-(leftb+upb)+lub;
        break;
    }
    return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
/**
   * Given a rectangle, compute its sum of squared deviations from avg,
   * over all pixels and all color channels. 
   * @param ul is (x,y) of the upper left corner of the rectangle 
   * @param w,h are the width and height of the rectangle
   */
    long sumr = getSum('r', ul, w, h); //r
    long sumg = getSum('g', ul, w, h); //g
    long sumb = getSum('b', ul, w, h); //b
    long sumrs = getSumSq('r', ul, w, h); //r
    long sumgs = getSumSq('g', ul, w, h); //g
    long sumbs = getSumSq('b', ul, w, h); //b
    double area = (w)*(h);
  
    // double sumx_2 = (double)(sumrs + sumgs + sumbs); 
    double sumr_x = sumrs-pow((double)(sumr),2)/area;
    double sumg_x = sumgs-pow((double)(sumg),2)/area;
    double sumb_x = sumbs - pow((double)(sumb),2)/area;

    double sumt_x = sumr_x + sumg_x + sumb_x;
    return sumt_x;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
 /**
   * Given a rectangle, return the average color value over the rectangle
   * as a pixel.
   * Each color component of the pixel is the average value of that
   * component over the rectangle.
   * @param ul is (x,y) of the upper left corner of the rectangle 
   * @param w,h are the width and height of the rectangle
   */
  double area = (w)*(h);
  long sumr = getSum('r', ul, w, h); //r
  long sumg = getSum('g', ul, w, h); //g
  long sumb = getSum('b', ul, w, h); //b
  RGBAPixel pix = RGBAPixel((int)sumr/area, (int)sumg/area, (int)sumb/area);
  return pix;
}
