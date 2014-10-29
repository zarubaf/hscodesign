Hardware/Software Codesign WS14/15
==========

decode_residuals:
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/flacdec.c#L209

decode_subframe_fixed:
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/flacdec.c#L259

decode_subframe_lpc:
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/flacdec.c#L310

'decoded' buffer:
+------------+-------------------------------------------+
| Warm up    | Residuals                                 |
+------------+-------------------------------------------+

'coeffs' buffer:
+-----------------+
|   netneiziffeoK |
+-----------------+

flac_lpc_16_c:
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/flacdsp.c#L46
