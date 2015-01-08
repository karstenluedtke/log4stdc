/**
 * @file      unconst.h
 *
 * @brief     BFC_UNCONST macro.
 *
 * @author    Karsten Luedtke
 *
 * @date      2015-01-08
 *
 * Copyright (c)  2015  Karsten Luedtke, Berlin, Germany.
 */

#ifndef BFC_UNCONST

#ifdef __cplusplus
#define BFC_UNCONST(ptrT,ptr)	const_cast<ptrT>(ptr)
#else
#define BFC_UNCONST(ptrT,ptr)	((ptrT)(size_t)(ptr))
#endif

#endif /* BFC_UNCONST */
