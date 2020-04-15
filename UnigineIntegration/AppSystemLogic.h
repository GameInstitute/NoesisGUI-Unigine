/* Copyright (C) 2005-2020, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.11 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#ifndef __APP_SYSTEM_LOGIC_H__
#define __APP_SYSTEM_LOGIC_H__

#define WIN32_LEAN_AND_MEAN
#include <UnigineLogic.h>
#include "NoesisGUI.h"

class AppSystemLogic : public Unigine::SystemLogic
{
public:
	AppSystemLogic();
	virtual ~AppSystemLogic();

	int init() override;

	int update() override;
	int postUpdate() override;

	//int render() override;

	int shutdown() override;

	NoesisGUI* noesisGUI;
};

#endif // __APP_SYSTEM_LOGIC_H__
