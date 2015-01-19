/*
 * complementary_filter.h
 *
 *  Created on: 2014-9-23
 *      Author: FGZ
 */

#ifndef COMPLEMENTARY_FILTER_H_
#define COMPLEMENTARY_FILTER_H_

void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch, float *roll);

#endif /* COMPLEMENTARY_FILTER_H_ */
