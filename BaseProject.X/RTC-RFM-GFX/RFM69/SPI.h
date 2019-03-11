/* 
 * File:   SPI.h
 * Author: kjfrischman
 *
 * Created on February 12, 2019, 1:35 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    int millis();
    int SPI_Transfer(int data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

