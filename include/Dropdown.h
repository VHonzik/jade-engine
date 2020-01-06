#pragma once

#include "ICompositeObject.h"
#include "ObjectLayer.h"

#include <cstdint>
#include <SDL.h>
#include <string>
#include <vector>

namespace JadeEngine
{
  class BoxSprite;
  class Sprite;
  class Text;

  struct DropdownParams
  {
    ObjectLayer     layer;
    std::string     boxTexture;
    int32_t         cornerSize;
    std::string     expandArrowTexture;
    std::string     contractArrowTexture;
    std::string     scrollBarTexture;
    std::string     scrollBarPointTexture;
    bool            spriteSheet;
    std::string     spriteSheetName;
    int32_t         width;
    int32_t         maxEntries;
    int32_t         entryHeight;
    int32_t         arrowsMargin;
    int32_t         scrollBarMargin;
    int32_t         z;
    std::string     textFont;
    uint32_t        textSize;
    SDL_Color       textColor;
  };

  class Dropdown : public ICompositeObject
  {
  public:
    Dropdown(const DropdownParams& params);

    void Update() override;

    template<typename InputIt>
    void AddEntries(InputIt first, InputIt last)
    {
      for (auto it = first; it != last; ++it)
      {
        AddEntry(*it);
      }

      UpdateEntries();
    }

    void SetPosition(int32_t x, int32_t y);
    void SetCenterPosition(int32_t x, int32_t y);

    void Show(const bool shown) override;

    int32_t GetX() const;
    int32_t GetY() const;

    int32_t GetCenterX() const;
    int32_t GetCenterY() const;

    int32_t GetWidth() const;
    int32_t GetHeight() const;

    bool Changed() const { return _changed; }
    int32_t GetIndex() const { return _currentEntry; }

    void SetIndex(int32_t index);

  private:
    void AddEntry(std::string text);
    void UpdateEntries();

    void Expand();
    void Contract();

    BoxSprite* _box;
    Sprite* _expandArrowSprite;
    Sprite* _contractArrowSprite;
    Sprite* _scrollBarSprite;
    Sprite* _scrollBarPointSprite;

    int32_t _maxVisibleEntries;
    int32_t _entryHeight;
    int32_t _arrowsMargin;
    int32_t _scrollBarMargin;

    std::string _fontName;
    uint32_t _fontSize;
    SDL_Color _fontColor;

    Text* _currentEntryText;
    int32_t _currentEntry;
    std::vector<Text*> _visibleEntriesTexts;
    std::vector<std::string> _entries;

    ObjectLayer _layer;

    bool _scrolling;
    int32_t _scrollOffset;

    bool _expanded;
    bool _changed;
  };

}
