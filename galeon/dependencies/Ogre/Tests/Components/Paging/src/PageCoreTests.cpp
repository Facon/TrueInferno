/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "PageCoreTests.h"
#include "OgrePaging.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PageCoreTests );

void PageCoreTests::setUp()
{
    // set up silent logging to not pollute output
	if(LogManager::getSingletonPtr())
		OGRE_DELETE Ogre::LogManager::getSingletonPtr();

	if(LogManager::getSingletonPtr() == 0)
	{
		LogManager* logManager = OGRE_NEW LogManager();
		logManager->createLog("testPageCore.log", true, false);
	}
    LogManager::getSingleton().setLogDetail(LL_LOW);

#if OGRE_STATIC
        mStaticPluginLoader = OGRE_NEW StaticPluginLoader();
#endif

#ifdef OGRE_STATIC_LIB
	mRoot = OGRE_NEW Root(StringUtil::BLANK);
        
	mStaticPluginLoader.load();
#else
	mRoot = OGRE_NEW Root();
#endif

    LogManager::getSingleton().setLogDetail(LL_LOW);
	mPageManager = OGRE_NEW PageManager();

	// make certain the resource location is NOT read-only
	ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem",
	    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false, false);

	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);

}

void PageCoreTests::tearDown()
{
	OGRE_DELETE mPageManager;
	OGRE_DELETE mRoot;
#if OGRE_STATIC
        OGRE_DELETE mStaticPluginLoader;
#endif
}


void PageCoreTests::testSimpleCreateSaveLoadWorld()
{
	String worldName = "MyWorld";
	String filename = "myworld.world";
	String sectionName1 = "Section1";
	String sectionName2 = "Section2";
	PagedWorld* world = mPageManager->createWorld(worldName);
	PagedWorldSection* section = world->createSection("Grid2D", mSceneMgr, sectionName1);
	section = world->createSection("Grid2D", mSceneMgr, sectionName2);

	// Create a page
	Page* p = section->loadOrCreatePage(Vector3::ZERO);

    p->createContentCollection("Simple");

	world->save(filename);

	mPageManager->destroyWorld(world);
	world = 0;
	world = mPageManager->loadWorld(filename);

	CPPUNIT_ASSERT_EQUAL(worldName, world->getName());
	CPPUNIT_ASSERT_EQUAL((size_t)2, world->getSectionCount());

	section = world->getSection(sectionName1);
	CPPUNIT_ASSERT(section != 0);
	section = world->getSection(sectionName2);
	CPPUNIT_ASSERT(section != 0);
}

