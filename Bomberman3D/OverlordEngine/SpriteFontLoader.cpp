#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "BinaryReader.h"
#include "ContentManager.h"
#include "TextureData.h"

SpriteFont* SpriteFontLoader::LoadContent(const std::wstring& assetFile)
{
	auto pBinReader = new BinaryReader();
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...
	if(pBinReader->Read<byte>() != 'B') {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Not a valid .fnt font", assetFile.c_str());
		return nullptr;
	}
	if (pBinReader->Read<byte>() != 'M') {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Not a valid .fnt font", assetFile.c_str());
		return nullptr;
	}
	if (pBinReader->Read<byte>() != 'F') {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Not a valid .fnt font", assetFile.c_str());
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...
	if (pBinReader->Read<byte>() < 3) {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported", assetFile.c_str());
		return nullptr;
	}

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	pBinReader->Read<byte>();
	pBinReader->Read<int>();
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	short FontSize = pBinReader->Read<short>();
	pSpriteFont->m_FontSize = FontSize;
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	//...
	pBinReader->MoveBufferPosition(12);
	std::wstring name = pBinReader->ReadNullString();
	pSpriteFont->m_FontName = name;
	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	pBinReader->Read<byte>();
	pBinReader->Read<int>();
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	pBinReader->MoveBufferPosition(4);
	UINT16 width = pBinReader->Read<UINT16>();
	UINT16 height = pBinReader->Read<UINT16>();
	pSpriteFont->m_TextureWidth = width;
	pSpriteFont->m_TextureHeight = height;
	//Retrieve PageCount
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	//...
	if(pBinReader->Read<UINT16>() > 1) {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed", assetFile.c_str());
		return nullptr;
	}
	pBinReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	pBinReader->Read<byte>();
	pBinReader->Read<int>();
	//Retrieve the PageName (store Local)
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	std::wstring pageName = pBinReader->ReadNullString();
	if(pageName.empty()) {
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]", assetFile.c_str());
		return nullptr;
	}
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	auto size = assetFile.rfind(L"Consolas_32.fnt");
	std::wstring filepath{};
	if(size != std::string::npos) {
		filepath = assetFile.substr(0, size);
	}
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	//...
	filepath += pageName;
	TextureData* texture = ContentManager::Load<TextureData>(filepath);
	pSpriteFont->m_pTexture = texture;
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	pBinReader->Read<byte>();
	int blockSize = pBinReader->Read<int>();
	//Retrieve Character Count (see documentation)
	int numChar = blockSize / 20;
	std::cout << numChar << "\n";
	//Retrieve Every Character, For every Character:
	for (int i{}; i < numChar; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		UINT32 characterIdUINT = pBinReader->Read<UINT32>();
		wchar_t characterId = wchar_t(characterIdUINT);
		//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		if(pSpriteFont->IsCharValid(characterId) == false)
		{
			Logger::LogFormat(LogLevel::Error, L"Char is not valid!", assetFile.c_str());
			continue;
		}
		//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
		FontMetric& fontMetric = pSpriteFont->GetMetric(characterId);
		//> Set IsValid to true [FontMetric::IsValid]
		fontMetric.IsValid = true;
		//> Set Character (CharacterId) [FontMetric::Character]
		fontMetric.Character = characterId;
		//> Retrieve Xposition (store Local)
		UINT16 Xposition = pBinReader->Read<UINT16>();
		//> Retrieve Yposition (store Local)
		UINT16 Yposition = pBinReader->Read<UINT16>();
		//> Retrieve & Set Width [FontMetric::Width]
		fontMetric.Width = pBinReader->Read<unsigned short>();
		//> Retrieve & Set Height [FontMetric::Height]
		fontMetric.Height = pBinReader->Read<unsigned short>();
		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		fontMetric.OffsetX = pBinReader->Read<short>();
		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		fontMetric.OffsetY = pBinReader->Read<short>();
		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		fontMetric.AdvanceX = pBinReader->Read<short>();
		//> Retrieve & Set Page [FontMetric::Page]
		fontMetric.Page = pBinReader->Read<unsigned char>();
		//> Retrieve Channel (BITFIELD!!!) 
		//	> See documentation for BitField meaning [FontMetrix::Channel]

		BYTE bitField = pBinReader->Read<BYTE>();
		int rightMostPos = int(log2(bitField & -bitField) + 1);
		switch (rightMostPos)
		{
		case 1:
			fontMetric.Channel = 2;
			break;
		case 2:
			fontMetric.Channel = 1;
			break;
		case 3:
			fontMetric.Channel = 0;
			break;
		case 4:
			fontMetric.Channel = 3;
			break;
		}


		//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
		//...
		fontMetric.TexCoord = DirectX::XMFLOAT2(Xposition / float(pSpriteFont->m_TextureWidth), Yposition / float(pSpriteFont->m_TextureHeight));

	}

	//DONE :)

	delete pBinReader;
	return pSpriteFont;
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
