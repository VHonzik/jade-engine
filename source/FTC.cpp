#include "FTC.h"

#include "Game.h"
#include "Text.h"
#include "Transform.h"

#include <numeric>
#include <cassert>

namespace JadeEngine
{
  FTC::FTC(const FTCParams& params)
    : _defaultColor(params.defaultColor)
    , _format(params.format)
    , Width(0)
    , Height(0)
    , _x(0)
    , _y(0)
    , _layer(params.layer)
    , _align(kHorizontalAlignment_Left)
    , _recalculateWanted(false)
  {
    _z = params.z;

    _font = GGame.FindFont(params.fontName, params.fontSize);

    assert(_font != nullptr);

    _subParams.color = _defaultColor;
    _subParams.fontName = params.fontName;
    _subParams.fontSize = params.fontSize;
    _subParams.layer = _layer;
    _subParams.text = "";
    _subParams.z = params.z;

    Rebuild();
  }

  void FTC::Rebuild()
  {
    for (const auto& text : _texts)
    {
      text->Clean();
    }

    _valueColors.clear();
    _texts.clear();

    uint32_t start = 0;
    for (uint32_t i = 0; i < _format.size(); i++)
    {
      if (_format[i] == '#' && (i == 0 || _format[i - 1] != '\\'))
      {
        if (start < i)
        {
          TextParams params = _subParams;
          params.text = _format.substr(start, i - start);

          _texts.push_back(GGame.Create<Text>(params));
        }

        _valueColors.push_back(_defaultColor);
        _valueIndex.push_back(_texts.size());

        TextParams params = _subParams;
        params.text = std::to_string(0);
        params.color = _valueColors[_valueColors.size() - 1];

        _texts.push_back(GGame.Create<Text>(params));

        start = i+1;
      }
    }

    if (start <= _format.size() - 1)
    {
      TextParams params = _subParams;
      params.text = _format.substr(start, _format.size() - start);

      _texts.push_back(GGame.Create<Text>(params));
    }

    _recalculateWanted = true;
  }

  void FTC::Update()
  {
    if (_recalculateWanted)
    {
      Recalculate();
    }
  }

  void FTC::SetIntValue(const uint32_t index, const int32_t value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(std::to_string(value));
    _recalculateWanted = true;
  }

  void FTC::SetStringValue(const uint32_t index, const std::string& value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(value);
    _recalculateWanted = true;
  }

  void FTC::SetFloatValue(const uint32_t index, const float value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(std::to_string(value));
    _recalculateWanted = true;
  }

  void FTC::SetValueColor(const uint32_t index, const SDL_Color& color)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetColor(color);
    _recalculateWanted = true;
  }

  void FTC::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    for (auto& text : _texts)
    {
      text->Show(isShown);
    }
  }

  void FTC::SetHorizontalAlign(const HorizontalAlignment align)
  {
    for (auto& text : _texts)
    {
      text->SetHorizontalAlign(align);
    }
  }

  void FTC::SetVerticalAlign(const VerticalAlignment align)
  {
    for (auto& text : _texts)
    {
      text->SetVerticalAlign(align);
    }
  }

  void FTC::SetPositon(int32_t x, int32_t y)
  {
    _x = x;
    _y = y;
    _recalculateWanted = true;
  }

  void FTC::Recalculate()
  {
    Width = 0;
    Height = 0;

    for (uint32_t i = 0; i < _texts.size(); i++)
    {
      auto& text = _texts[i];
      text->SetPosition(i == 0 ? _x : _texts[i - 1]->GetX() + _texts[i - 1]->GetWidth(), _y);
      Width += text->GetWidth();
      Height = std::max(Height, text->GetHeight());
    }

    _recalculateWanted = false;
  }
}