#ifndef __DESCRIPTOR_H__
#define	__DESCRIPTOR_H__

const unsigned char descriptor[] = {

/* Device Descriptor */
0x12,	         /* Descriptor length */
0x01,          /* Decriptor type */
0x00,0x02,     /* Specification Version (BCD) */
0x00,          /* Device class */
0x00,          /* Device sub-class */
0x00,          /* Device sub-sub-class */
0x40,          /* Maximum packet size */
0x47, 0x05,    /* Vendor ID */
0x02, 0x10,    /* Product ID (Sample Device) */
0x00, 0x00,    /* Product version ID */
0x01,          /* Manufacturer string index */
0x02,          /* Product string index */
0x00,          /* Serial number string index */
0x01,          /* Number of configurations */

/* Device Qualifier Descriptor */
0x0a,          /* Descriptor length */
0x06,          /* Decriptor type */
0x00, 0x02,    /* Specification Version (BCD) */
0x00,          /* Device class */
0x00,          /* Device sub-class */
0x00,          /* Device sub-sub-class */
0x40,          /* Maximum packet size */
0x01,          /* Number of configurations */
0x00,          /* Reserved */

/* High-Speed Configuration Descriptor */
0x09,          /* Descriptor length */
0x02,          /* Descriptor type */
0x20,          /* Total Length (LSB) */
0x00,          /* Total Length (MSB) */
0x01,          /* Number of interfaces */
0x01,          /* Configuration number */
0x00,          /* Configuration string */
0xA0,          /* Attributes (b7 - buspwr, b6 - selfpwr, b5 - rwu) */
0x32,          /* Power requirement (div 2 ma) */

/* Interface Descriptor */
0x09,          /* Descriptor length */
0x04,          /* Descriptor type */
0x00,          /* Zero-based index of this interface */
0x00,          /* Alternate setting */
0x02,          /* Number of end points */
0xFF,          /* Interface class */
0x00,          /* Interface sub class */
0x00,          /* Interface sub sub class */
0x00,          /* Interface descriptor string index */

/* Endpoint 2 Descriptor */
0x07,          /* Descriptor length */
0x05,          /* Descriptor type */
0x02,          /* Endpoint number, and direction */
0x02,          /* Endpoint type */
0x40,          /* Maximun packet size (LSB) */
0x00,          /* Max packect size (MSB) */
0x00,          /* Polling interval */

/* Endpoint 4 Descriptor */
0x07,          /* Descriptor length */
0x05,          /* Descriptor type */
0x84,          /* Endpoint number, and direction */
0x02,          /* Endpoint type */
0x40,          /* Maximun packet size (LSB) */
0x00,          /* Max packect size (MSB) */
0x00,          /* Polling interval */

/* Full-Speed Configuration Descriptor */
0x09,          /* Descriptor length */
0x02,          /* Descriptor type */
0x20,          /* Total Length (LSB) */
0x00,          /* Total Length (MSB) */
0x01,          /* Number of interfaces */
0x01,          /* Configuration number */
0x00,          /* Configuration string */
0xA0,          /* Attributes (b7 - buspwr, b6 - selfpwr, b5 - rwu) */
0x32,          /* Power requirement (div 2 ma) */

/* Interface Descriptor */
0x09,          /* Descriptor length */
0x04,          /* Descriptor type */
0x00,          /* Zero-based index of this interface */
0x00,          /* Alternate setting */
0x02,          /* Number of end points */
0xFF,          /* Interface class */
0x00,          /* Interface sub class */
0x00,          /* Interface sub sub class */
0x00,          /* Interface descriptor string index */

/// EP2 OUT PC -> CY
/* Endpoint 2 Descriptor */
0x07,          /* Descriptor length */
0x05,          /* Descriptor type */
0x02,          /* Endpoint number, and direction */
0x02,          /* Endpoint type */
0x40,          /* Maximun packet size (LSB) */
0x00,          /* Max packect size (MSB) */
0x00,          /* Polling interval */

/// EP4 IN PC <- CY
/* Endpoint 4 Descriptor */
0x07,          /* Descriptor length */
0x05,          /* Descriptor type */
0x84,          /* Endpoint number, and direction */
0x02,          /* Endpoint type */
0x40,          /* Maximun packet size (LSB) */
0x00,          /* Max packect size (MSB) */
0x00,          /* Polling interval */

/* String Descriptor 0 */
2+2,
0x03,
0x09, 0x04,

/* String Descriptor 1 */
2+(2*18),
0x03,
'w', 0x00,
'w', 0x00,
'w', 0x00,
'.', 0x00,
's', 0x00,
'p', 0x00,
'e', 0x00,
'c', 0x00,
'c', 0x00,
'y', 0x00,
'l', 0x00,
'a', 0x00,
'n', 0x00,
'd', 0x00,
'.', 0x00,
'n', 0x00,
'e', 0x00,
't', 0x00,

/* String Descriptor 2 */
2+(2*18),
0x03,
'N', 0x00,
'a', 0x00,
'n', 0x00,
'o', 0x00,
'B', 0x00,
'o', 0x00,
'a', 0x00,
'r', 0x00,
'd', 0x00,
' ', 0x00,
'N', 0x00,
'B', 0x00,
'3', 0x00,
'0', 0x00,
'0', 0x00,
'0', 0x00,
'A', 0x00,
'L', 0x00
};

#endif
