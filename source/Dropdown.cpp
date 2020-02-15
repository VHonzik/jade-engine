#include "Dropdown.h"

#include "BoxSprite.h"
#include "Game.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "Transform.h"
#include "Utils.h"

namespace JadeEngine
{
  Dropdown::Dropdown(const DropdownParams& params)
    : _expanded(false)
    , _entryHeight(params.entryHeight)
    , _arrowsMargin(params.arrowsMargin)
    , _fontName(params.textFont)
    , _fontSize(params.textSize)
    , _fontColor(params.textColor)
    , _currentEntry(0)
    , _changed(false)
    , _layer(params.layer)
    , _scrollBarMargin(params.scrollBarMargin)
    , _maxVisibleEntries(params.maxEntries)
    , _scrolling(false)
    , _scrollOffset(0)
  {
    BoxSpriteParams boxParams;
    boxParams.layer = params.layer;
    boxParams.textureName = params.boxTexture;
    boxParams.z = params.z - 1;
    boxParams.spriteSheet = params.spriteSheet;
    boxParams.spriteSheetName = params.spriteSheetName;
    boxParams.cornerSize = params.cornerSize;
    boxParams.size = { params.width, params.entryHeight};

    _box = GGame.Create<BoxSprite>(boxParams);

    SpriteParams spriteParams;
    spriteParams.layer = params.layer;
    spriteParams.textureName = params.expandArrowTexture;
    spriteParams.z = params.z - 1;
    spriteParams.spriteSheet = params.spriteSheet;
    spriteParams.spriteSheetName = params.spriteSheetName;

    _expandArrowSprite = GGame.Create<Sprite>(spriteParams);

    spriteParams.textureName = params.contractArrowTexture;
    _contractArrowSprite = GGame.Create<Sprite>(spriteParams);
    _contractArrowSprite->Show(false);

    spriteParams.textureName = params.scrollBarTexture;
    _scrollBarSprite = GGame.Create<Sprite>(spriteParams);
    _scrollBarSprite->Show(false);

    spriteParams.textureName = params.scrollBarPointTexture;
    _scrollBarPointSprite = GGame.Create<Sprite>(spriteParams);
    _scrollBarPointSprite->Show(false);

    TextParams textParams;
    textParams.layer = params.layer;
    textParams.fontName = _fontName;
    textParams.fontSize = _fontSize;
    textParams.text = "";
    textParams.color = _fontColor;
    textParams.z = params.z;

    _currentEntryText = GGame.Create<Text>(textParams);
    _box->transform->Attach(_currentEntryText->transform, kZeroVector2D_i32, kAnchor_leftCenter, kAnchor_leftCenter);

    SetPosition(0, 0);
  }

  void Dropdown::AddEntry(std::string text)
  {
    _entries.push_back(text);

    if (_entries.size() <= _maxVisibleEntries)
    {
      TextParams textParams;
      textParams.layer = _layer;
      textParams.fontName = _fontName;
      textParams.fontSize = _fontSize;
      textParams.text = text;
      textParams.color = _fontColor;
      textParams.z = _box->GetZ() + 1;

      _visibleEntriesTexts.push_back(GGame.Create<Text>(textParams));
      auto newEntry = _visibleEntriesTexts[_visibleEntriesTexts.size() - 1];
      _box->transform->Attach(newEntry->transform, kZeroVector2D_i32, kAnchor_leftCenter, kAnchor_leftCenter);
    }
  }

  void Dropdown::SetIndex(int32_t index)
  {
    if (index >= 0 && index < _entries.size())
    {
      _currentEntry = index;
    }
  }

  void Dropdown::Expand()
  {
    const auto size = std::min(static_cast<int32_t>(_visibleEntriesTexts.size()+1), _maxVisibleEntries+1);
    _box->transform->SetHeight(_entryHeight * size);
    for (int32_t i = 0; i < _visibleEntriesTexts.size(); i++)
    {
      _visibleEntriesTexts[i]->Show(_shown);
    }
    _contractArrowSprite->Show(true);
    _expandArrowSprite->Show(false);

    _scrollBarSprite->transform->SetHeight(_entryHeight * (size-1) - _scrollBarMargin);
    _scrollBarSprite->Show(true);
    _scrollBarPointSprite->Show(true);
  }

  void Dropdown::Contract()
  {
    _box->transform->SetHeight(_entryHeight);
    for (int32_t i = 0; i < _visibleEntriesTexts.size(); i++)
    {
      _visibleEntriesTexts[i]->Show(false);
    }
    _contractArrowSprite->Show(false);
    _expandArrowSprite->Show(true);
    _scrollBarSprite->Show(false);
    _scrollBarPointSprite->Show(false);
  }

  void Dropdown::Update()
  {
    _changed = false;

    auto hoveredSprite = GGame.GetHoveredSprite();
    if (!_expanded && (hoveredSprite == _box || hoveredSprite == _expandArrowSprite) && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      _expanded = true;
      Expand();
    }
    else if (_expanded && (hoveredSprite == _box || hoveredSprite == _contractArrowSprite) && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      auto index = (GInput.GetMouseY() - GetY()) / _entryHeight;
      bool scrollBar = GInput.GetMouseX() > _box->transform->GetX() + _box->transform->GetWidth() - 2 * _scrollBarMargin - _scrollBarPointSprite->transform->GetWidth();
      if (index == 0)
      {
        _expanded = false;
        Contract();
      }
      else if (scrollBar)
      {
        _scrolling = true;
      }
      else if (_scrollOffset + (index - 1) == _currentEntry)
      {
        _expanded = false;
        Contract();
      }
      else
      {
        _currentEntry = _scrollOffset + index-1;
        _expanded = false;
        _changed = true;
        Contract();
        UpdateEntries();
      }
    }
    else if (_expanded && _scrolling)
    {
      if (GInput.MouseButtonDown(SDL_BUTTON_LEFT))
      {
        const auto y = Clamp(GInput.GetMouseY(), _scrollBarSprite->transform->GetY(), _scrollBarSprite->transform->GetY() + _scrollBarSprite->transform->GetHeight());
        _scrollBarPointSprite->transform->SetCenterPosition(_scrollBarSprite->transform->GetCenterX(), y);
        const auto t = (static_cast<float>(y) - _scrollBarSprite->transform->GetY()) / _scrollBarSprite->transform->GetHeight();
        const auto index = static_cast<int32_t>((_entries.size()-1-_maxVisibleEntries) * t);
        _scrollOffset = index;
        UpdateEntries();
      }
      else
      {
        _scrolling = false;
      }
    }
    else if (_expanded && GInput.GetMouseWheelY() != 0)
    {
      _scrollOffset = Clamp(_scrollOffset + GInput.GetMouseWheelY(), 0, static_cast<int32_t>(_entries.size() - 1 - _maxVisibleEntries));
      const auto t = static_cast<float>(_scrollOffset) / (_entries.size() - 1 - _maxVisibleEntries);
      const auto y = static_cast<int32_t>(_scrollBarSprite->transform->GetY() + t * _scrollBarSprite->transform->GetHeight());
      _scrollBarPointSprite->transform->SetCenterPosition(_scrollBarSprite->transform->GetCenterX(), y);

      UpdateEntries();
    }
    else if (_expanded && (hoveredSprite != _box && hoveredSprite != _contractArrowSprite) && GInput.MouseButtonPressed(SDL_BUTTON_LEFT))
    {
      _expanded = false;
      Contract();
    }
  }

  void Dropdown::UpdateEntries()
  {
    _currentEntryText->Show(_shown);
    _currentEntryText->transform->SetLocalPosition(_arrowsMargin, _entryHeight / 2);

    if (_currentEntry < _entries.size())
    {
      _currentEntryText->SetText(_entries[_currentEntry]);
    }
    else
    {
      _currentEntryText->SetText("");
    }

    for (int32_t i = 0; i < _visibleEntriesTexts.size(); i++)
    {
      _visibleEntriesTexts[i]->Show(_shown && _expanded);
      _visibleEntriesTexts[i]->transform->SetLocalPosition(_arrowsMargin, (_entryHeight / 2) + (i + 1) * _entryHeight);
      if (_visibleEntriesTexts[i]->GetText() != _entries[_scrollOffset + i])
      {
        _visibleEntriesTexts[i]->SetText(_entries[_scrollOffset + i]);
      }
    }
  }

  void Dropdown::Show(bool shown)
  {
    IGameObject::Show(shown);
    const auto isShown = IsShown();

    _box->Show(isShown);
    _expandArrowSprite->Show(isShown && !_expanded);
    _contractArrowSprite->Show(isShown && _expanded);
    _scrollBarSprite->Show(isShown && _expanded);
    _scrollBarPointSprite->Show(isShown && _expanded);
    UpdateEntries();
  }

  int32_t Dropdown::GetX() const
  {
    return _box->transform->GetX();
  }

  int32_t Dropdown::GetY() const
  {
    return _box->transform->GetY();
  }

  int32_t Dropdown::GetCenterX() const
  {
    return _box->transform->GetCenterX();
  }

  int32_t Dropdown::GetCenterY() const
  {
    return _box->transform->GetCenterY();
  }

  int32_t Dropdown::GetWidth() const
  {
    return _box->transform->GetWidth();
  }

  int32_t Dropdown::GetHeight() const
  {
    return _entryHeight;
  }

  void Dropdown::SetPosition(int32_t x, int32_t y)
  {
    _box->transform->SetPosition(x, y);
    _expandArrowSprite->transform->SetCenterPosition(_box->transform->GetX() + _box->transform->GetWidth() - _expandArrowSprite->transform->GetWidth() / 2 - _arrowsMargin, _box->transform->GetY() + _entryHeight / 2);
    _contractArrowSprite->transform->SetCenterPosition(_expandArrowSprite->transform->GetCenterX(), _expandArrowSprite->transform->GetCenterY());

    _scrollBarSprite->transform->SetPosition(_box->transform->GetX() + _box->transform->GetWidth() - _scrollBarSprite->transform->GetWidth() - _scrollBarMargin, _box->transform->GetY() + _entryHeight);

    _scrollBarPointSprite->transform->SetCenterPosition(_scrollBarSprite->transform->GetCenterX(), _scrollBarSprite->transform->GetY());

    UpdateEntries();
  }

  void Dropdown::SetCenterPosition(int32_t x, int32_t y)
  {
    _box->transform->SetCenterPosition(x, y);
    _expandArrowSprite->transform->SetCenterPosition(_box->transform->GetX() + _box->transform->GetWidth() - _expandArrowSprite->transform->GetWidth() / 2 - _arrowsMargin, _box->transform->GetY() + _entryHeight / 2);
    _contractArrowSprite->transform->SetCenterPosition(_expandArrowSprite->transform->GetCenterX(), _expandArrowSprite->transform->GetCenterY());

    _scrollBarSprite->transform->SetPosition(_box->transform->GetX() + _box->transform->GetWidth() - _scrollBarSprite->transform->GetWidth() - _scrollBarMargin, _box->transform->GetY() + _entryHeight);

    _scrollBarPointSprite->transform->SetCenterPosition(_scrollBarSprite->transform->GetCenterX(), _scrollBarSprite->transform->GetY());

    UpdateEntries();
  }
}