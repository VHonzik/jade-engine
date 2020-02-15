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
    , _layer(params.layer)
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

    transform->Initialize(kZeroVector2D_i32, kZeroVector2D_i32);

    Rebuild();
  }

  void FTC::Rebuild()
  {
    for (const auto& text : _texts)
    {
      if (text->transform->IsAttached()) text->transform->Detach();
      text->Destroy();
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

    assert(_texts.size() > 0);
    transform->Attach(_texts[0]->transform);

    for (size_t i = 1; i < _texts.size(); i++)
    {
      _texts[i - 1]->transform->Attach(_texts[i]->transform, kZeroVector2D_i32, kAnchor_rightBottom, kAnchor_leftBottom);
    }

    const auto size = std::accumulate(std::cbegin(_texts), std::cend(_texts), kZeroVector2D_i32, [&](const Vector2D_i32& a, const Text* b)
    {
      return Vector2D_i32{ a.x + b->transform->GetWidth(), std::max(a.y ,b->transform->GetHeight()) };
    });
    transform->SetSize(size);
  }

  void FTC::SetIntValue(const uint32_t index, const int32_t value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(std::to_string(value));
  }

  void FTC::SetStringValue(const uint32_t index, const std::string& value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(value);
  }

  void FTC::SetFloatValue(const uint32_t index, const float value)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetText(std::to_string(value));
  }

  void FTC::SetValueColor(const uint32_t index, const SDL_Color& color)
  {
    const auto textIndex = _valueIndex[index];
    _texts[textIndex]->SetColor(color);
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
}