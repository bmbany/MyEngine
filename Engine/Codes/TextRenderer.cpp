#include "TextRenderer.h"
#include "GraphicManager.h"

using namespace Engine;

Engine::TextRenderer::TextRenderer(const wchar_t* name, DWORD color, float size, DWRITE_FONT_WEIGHT fontWeight)
	: Component(name)
	, _fontSize(size)
	, _fontWeight(fontWeight)
{	
	GraphicManager* pGraphicMgr = GraphicManager::GetInstance();
	/*_pSolidColorBrush = pGraphicMgr->GetSolidColorBrush();
	_pDeviceContext = pGraphicMgr->GetDeviceContext();
	_pWriteFactory = pGraphicMgr->GetWriteFactory();*/

	_pWriteFactory->CreateTextFormat(
		L"¸¼Àº °íµñ", NULL,
		_fontWeight, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		_fontSize, L"", &_pWriteTextFormat);
}

void TextRenderer::Render()
{
}

void Engine::TextRenderer::SetDrawRect(float width, float height)
{
	_drawRect = { 0.f, 0.f, width, height };
	_drawPoint = { width, height };
}

void Engine::TextRenderer::SetTextLayout(const wchar_t* text, float width, float height)
{
	_pWriteFactory->CreateTextLayout(text, lstrlen(text), _pWriteTextFormat, width, height, &_pWriteTextLayout);
}

void Engine::TextRenderer::SetTextRangeEffectColor(unsigned int start, unsigned int length)
{
}

void Engine::TextRenderer::SetTextRangeEffectBold(unsigned int start, unsigned int length)
{
	if (nullptr == _pWriteTextLayout)
		return;
	
	DWRITE_TEXT_RANGE textRange = { start, length };
	_pWriteTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, textRange);
}

void Engine::TextRenderer::SetTextRangeEffectUnderline(unsigned int start, unsigned int length)
{
	if (nullptr == _pWriteTextLayout)
		return;

	DWRITE_TEXT_RANGE textRange = { start, length };
	_pWriteTextLayout->SetUnderline(TRUE, textRange);
}

void Engine::TextRenderer::SetTextRangeEffectFontFamily(unsigned int start, unsigned int length, const wchar_t* fontFamily)
{
	DWRITE_TEXT_RANGE textRange = { start, length };
	_pWriteTextLayout->SetFontFamilyName(fontFamily, textRange);
}

void Engine::TextRenderer::SetTextAlignment(DWRITE_TEXT_ALIGNMENT alignment)
{
	_pWriteTextFormat->SetTextAlignment(alignment);
}

void Engine::TextRenderer::SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
	_pWriteTextFormat->SetParagraphAlignment(alignment);
}

void TextRenderer::Free()
{
	SafeRelease(_pWriteTextFormat);
}
