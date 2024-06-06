//
// Created by marie on 02/05/2024.
//

#include "game/assets.h"
#include <filesystem>
#include "raygui.h"
#include "game/player.h"

#include <iostream>
#include "rres/rres.h"
#include "rres/rres-raylib.h"


Player playerAssets = Player::getInstance();

class Assets;

Texture2D LoadTextureFilterFromArchive(const std::filesystem::path& rresPath, const std::string& texturePath, int& loadedAssets)
{
    Texture2D texture = {0};

    rresCentralDir dir = rresLoadCentralDirectory(rresPath.string().c_str());
    unsigned int id = rresGetResourceId(dir, texturePath.c_str());
    rresResourceChunk chunk = rresLoadResourceChunk(rresPath.string().c_str(), id);
    int result = UnpackResourceChunk(&chunk);

    if (result == RRES_SUCCESS)
    {
        Image image = LoadImageFromResource(chunk);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    rresUnloadResourceChunk(chunk);
    rresUnloadCentralDirectory(dir);

    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);
    loadedAssets++;

    return texture;
}

// TODO ermilburn02 - loading models from archive

// TODO ermilburn02 - loading fonts from archive

Texture2D Assets::LoadTextureFilter(const std::filesystem::path &texturePath, int& loadedAssets) {
    Texture2D tex = LoadTexture(texturePath.string().c_str());
    GenTextureMipmaps(&tex);
    SetTextureFilter(tex, TEXTURE_FILTER_TRILINEAR);
    loadedAssets++;
    return tex;
}

Model Assets::LoadModel_(const std::filesystem::path& modelPath, int& loadedAssets) {
    return LoadModel(modelPath.string().c_str());
    loadedAssets++;
}

Font Assets::LoadFontFilter(const std::filesystem::path &fontPath, int fontSize, int& loadedAssets) {
    Font font = LoadFontEx(fontPath.string().c_str(), fontSize, 0,0);
    font.baseSize = 128;
    font.glyphCount = 95;
    int fileSize = 0;
    unsigned char* fileData = LoadFileData(fontPath.string().c_str(), &fileSize);
    font.glyphs = LoadFontData(fileData, fileSize, 128, 0, 95, FONT_SDF);
    Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, 95, 128, 4, 1);
    font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);
    loadedAssets++;
    return font;
}
void Assets::FirstAssets() {
    icon = LoadImage((directory / "Assets/encore_favicon-NEW.png").string().c_str());
    // encoreWhiteLogo = Assets::LoadTextureFilter((directory / "Assets/encore-white.png"), loadedAssets);
    // encoreWhiteLogo = Assets::LoadTextureFilter(directory, "Assets/encore-white.png", loadedAssets);
    encoreWhiteLogo = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/encore-white.png", loadedAssets);
    rubik = Assets::LoadFontFilter((directory / "Assets/fonts/Rubik-Regular.ttf"), 256, loadedAssets);
}
void Assets::LoadAssets() {
    smasherReg = Assets::LoadModel_((directory / "Assets/highway/smasher.obj"), loadedAssets);
    // smasherRegTex = Assets::LoadTextureFilter(directory / "Assets/highway/smasher_reg.png", loadedAssets);
    smasherRegTex = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/smasher_reg.png", loadedAssets);

    // smasherBoardTex = Assets::LoadTextureFilter(directory / "Assets/highway/board.png", loadedAssets);
    smasherBoardTex = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/board.png", loadedAssets);
    smasherBoard = Assets::LoadModel_((directory / "Assets/highway/board_x.obj"), loadedAssets);
    smasherBoardEMH = Assets::LoadModel_((directory / "Assets/highway/board_emh.obj"), loadedAssets);

    lanes = Assets::LoadModel_((directory / "Assets/highway/lanes.obj"), loadedAssets);
    // lanesTex = Assets::LoadTextureFilter(directory / "Assets/highway/lanes.png", loadedAssets);
    lanesTex = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/lanes.png", loadedAssets);

    smasherPressed = Assets::LoadModel_((directory / "Assets/highway/smasher.obj"), loadedAssets);
    // smasherPressTex = Assets::LoadTextureFilter(directory / "Assets/highway/smasher_press.png", loadedAssets);
    smasherPressTex = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/smasher_press.png", loadedAssets);

    // star = Assets::LoadTextureFilter(directory/ "Assets/ui/star.png", loadedAssets);
    star = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/star.png", loadedAssets);
    // goldStar = Assets::LoadTextureFilter(directory/ "Assets/ui/gold-star.png", loadedAssets);
    goldStar = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/gold-star.png", loadedAssets);
    // goldStarUnfilled = Assets::LoadTextureFilter(directory/ "Assets/ui/gold-star_unfilled.png", loadedAssets);
    goldStarUnfilled = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/gold-star_unfilled.png", loadedAssets);
    // emptyStar = Assets::LoadTextureFilter(directory/ "Assets/ui/empty-star.png", loadedAssets);
    emptyStar = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/empty-star.png", loadedAssets);

    odFrame = Assets::LoadModel_((directory / "Assets/ui/od_frame.obj"), loadedAssets);
    odBar = Assets::LoadModel_((directory / "Assets/ui/od_fill.obj"), loadedAssets);
    multFrame = Assets::LoadModel_((directory / "Assets/ui/multcircle_frame.obj"), loadedAssets);
    multBar = Assets::LoadModel_((directory / "Assets/ui/multcircle_fill.obj"), loadedAssets);
    multCtr3 = Assets::LoadModel_((directory / "Assets/ui/multbar_3.obj"), loadedAssets);
    multCtr5 = Assets::LoadModel_((directory / "Assets/ui/multbar_5.obj"), loadedAssets);
    multNumber = Assets::LoadModel_((directory / "Assets/ui/mult_number_plane.obj"), loadedAssets);
    // odMultFrame = Assets::LoadTextureFilter(directory / "Assets/ui/mult_base.png", loadedAssets);
    odMultFrame = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/mult_base.png", loadedAssets);
    // odMultFill = Assets::LoadTextureFilter(directory / "Assets/ui/mult_fill.png", loadedAssets);
    odMultFill = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/mult_fill.png", loadedAssets);
    // odMultFillActive = Assets::LoadTextureFilter(directory / "Assets/ui/mult_fill_od.png", loadedAssets);
    odMultFillActive = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/mult_fill_od.png", loadedAssets);
    // multNumberTex = Assets::LoadTextureFilter(directory / "Assets/ui/mult_number.png", loadedAssets);
    multNumberTex = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/mult_number.png", loadedAssets);
    odMultShader = LoadShader(0, "Assets/ui/odmult.fs");
    multNumberShader = LoadShader(0, "Assets/ui/multnumber.fs");

    odLoc = GetShaderLocation(odMultShader, "overdrive");
    comboCounterLoc = GetShaderLocation(odMultShader, "comboCounter");
    multLoc = GetShaderLocation(odMultShader, "multBar");
    isBassOrVocalLoc = GetShaderLocation(odMultShader, "isBassOrVocal");
    uvOffsetXLoc = GetShaderLocation(multNumberShader, "uvOffsetX");
    uvOffsetYLoc = GetShaderLocation(multNumberShader, "uvOffsetY");


    expertHighwaySides = Assets::LoadModel_(directory / "Assets/highway/sides_x.obj", loadedAssets);
    expertHighway = Assets::LoadModel_((directory / "Assets/highway/highway_x.obj"), loadedAssets);
    emhHighwaySides = Assets::LoadModel_((directory / "Assets/highway/sides_emh.obj"), loadedAssets);
    emhHighway = Assets::LoadModel_((directory / "Assets/highway/highway_emh.obj"), loadedAssets);
    odHighwayEMH = Assets::LoadModel_((directory / "Assets/highway/overdrive_emh.obj"), loadedAssets);
    odHighwayX = Assets::LoadModel_((directory / "Assets/highway/overdrive_x.obj"), loadedAssets);
    // highwayTexture = Assets::LoadTextureFilter(directory / "Assets/highway/highway.png", loadedAssets);
    highwayTexture = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/highway.png", loadedAssets);
    // highwayTextureOD = Assets::LoadTextureFilter(directory / "Assets/highway/overdrive.png", loadedAssets);
    highwayTextureOD = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/overdrive.png", loadedAssets);
    // highwaySidesTexture = Assets::LoadTextureFilter(directory / "Assets/highway/sides.png", loadedAssets);
    highwaySidesTexture = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/highway/sides.png", loadedAssets);

    noteModel = Assets::LoadModel_((directory / "Assets/notes/note.obj"), loadedAssets);
    // noteTexture = Assets::LoadTextureFilter(directory / "Assets/notes/note.png", loadedAssets);
    noteTexture = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/notes/note.png", loadedAssets);
    // emitTexture = Assets::LoadTextureFilter(directory / "Assets/notes/note_e_new.png", loadedAssets);
    emitTexture = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/notes/note_e_new.png", loadedAssets);

    noteModelOD = Assets::LoadModel_((directory / "Assets/notes/note.obj"), loadedAssets);
    // noteTextureOD = Assets::LoadTextureFilter(directory / "Assets/notes/note.png", loadedAssets);
    noteTextureOD = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/notes/note.png", loadedAssets);
    // emitTextureOD = Assets::LoadTextureFilter(directory / "Assets/notes/note_e_new.png", loadedAssets);
    emitTextureOD = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/notes/note_e_new.png", loadedAssets);

    liftModel = Assets::LoadModel_((directory / "Assets/notes/lift.obj"), loadedAssets);
    liftModelOD = Assets::LoadModel_((directory / "Assets/notes/lift.obj"), loadedAssets);


    // songBackground = Assets::LoadTextureFilter((directory / "Assets/background.png"), loadedAssets);
    songBackground = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/background.png", loadedAssets);

    redHatDisplayItalic = Assets::LoadFontFilter((directory/"Assets/fonts/RedHatDisplay-BlackItalic.ttf"), 256, loadedAssets);
    redHatDisplayItalicLarge = Assets::LoadFontFilter((directory/"Assets/fonts/RedHatDisplay-BlackItalic.ttf"), 256, loadedAssets);
    redHatDisplayBlack = Assets::LoadFontFilter((directory/"Assets/fonts/RedHatDisplay-Black.ttf"), 256, loadedAssets);

    rubikBoldItalic = Assets::LoadFontFilter((directory / "Assets/fonts/Rubik-BoldItalic.ttf"), 256, loadedAssets);
    rubikBold = Assets::LoadFontFilter((directory / "Assets/fonts/Rubik-Bold.ttf"), 256, loadedAssets);
    rubikItalic = Assets::LoadFontFilter((directory / "Assets/fonts/Rubik-Italic.ttf"), 256, loadedAssets);

    josefinSansItalic = Assets::LoadFontFilter((directory / "Assets/fonts/JosefinSans-Italic.ttf"), 256, loadedAssets);

    sdfShader = LoadShader(0, (directory / "Assets/fonts/sdf.fs").string().c_str());
    bgShader = LoadShader(0, (directory / "Assets/ui/wavy.fs").string().c_str());
    bgTimeLoc= GetShaderLocation(bgShader, "time");
    //clapOD = LoadSound((directory / "Assets/highway/clap.ogg"));
    //SetSoundVolume(clapOD, 0.375);

    // discord = Assets::LoadTextureFilter(directory/"Assets/ui/discord-mark-white.png", loadedAssets);
    discord = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/discord-mark-white.png", loadedAssets);
    // github = Assets::LoadTextureFilter(directory/"Assets/ui/github-mark-white.png", loadedAssets);
    github = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/ui/github-mark-white.png", loadedAssets);

    // sustainTexture = Assets::LoadTextureFilter(directory / "Assets/notes/sustain.png", loadedAssets);
    sustainTexture = LoadTextureFilterFromArchive((directory / "Resources.rres"), "Assets/notes/sustain.png", loadedAssets);

    smasherReg.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = smasherRegTex;
    smasherReg.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    smasherReg.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = smasherRegTex;
    smasherReg.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    smasherPressed.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = smasherPressTex;
    smasherPressed.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    smasherBoard.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = smasherBoardTex;
    smasherBoard.materials[0].maps[MATERIAL_MAP_ALBEDO].color = LIGHTGRAY;

    smasherBoardEMH.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = smasherBoardTex;
    smasherBoardEMH.materials[0].maps[MATERIAL_MAP_ALBEDO].color = LIGHTGRAY;

    lanes.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = lanesTex;
    lanes.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    odFrame.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    odFrame.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    odBar.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    odBar.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    multFrame.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    multFrame.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    multBar.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    multBar.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    multCtr3.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    multCtr3.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    multCtr5.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = odMultFrame;
    multCtr5.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    odBar.materials[0].maps[MATERIAL_MAP_EMISSION].texture = odMultFill;
    multBar.materials[0].maps[MATERIAL_MAP_EMISSION].texture = odMultFill;
    multCtr3.materials[0].maps[MATERIAL_MAP_EMISSION].texture = odMultFill;
    multCtr5.materials[0].maps[MATERIAL_MAP_EMISSION].texture = odMultFill;
    odBar.materials[0].shader = odMultShader;
    multBar.materials[0].shader = odMultShader;
    multCtr3.materials[0].shader = odMultShader;
    multCtr5.materials[0].shader = odMultShader;

    odMultShader.locs[SHADER_LOC_MAP_EMISSION] = GetShaderLocation(odMultShader, "fillTex");

    multNumber.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = multNumberTex;
    multNumber.materials[0].shader = multNumberShader;

    SetTextureWrap(highwayTextureOD, TEXTURE_WRAP_CLAMP);
    odHighwayX.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwayTextureOD;
    odHighwayEMH.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwayTextureOD;

    odHighwayX.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.overdriveColor;
    odHighwayEMH.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.overdriveColor;
    expertHighwaySides.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwaySidesTexture;
    expertHighwaySides.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    expertHighway.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwayTexture;
    expertHighway.materials[0].maps[MATERIAL_MAP_ALBEDO].color = DARKGRAY;
    emhHighwaySides.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwaySidesTexture;
    emhHighwaySides.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    emhHighway.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = highwayTexture;
    emhHighway.materials[0].maps[MATERIAL_MAP_ALBEDO].color = DARKGRAY;

    noteModel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = noteTexture;
    noteModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    noteModel.materials[0].maps[MATERIAL_MAP_EMISSION].texture = emitTexture;
    noteModel.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
    noteModel.materials[0].maps[MATERIAL_MAP_EMISSION].value = 1;

    noteModelOD.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = noteTextureOD;
    noteModelOD.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    noteModelOD.materials[0].maps[MATERIAL_MAP_EMISSION].texture = emitTextureOD;
    noteModelOD.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;

    liftModel.materials[0].maps[MATERIAL_MAP_ALBEDO].color = playerAssets.accentColor;
    liftModelOD.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    sustainMat = LoadMaterialDefault();
    sustainMatHeld = LoadMaterialDefault();
    sustainMatOD = LoadMaterialDefault();
    sustainMatHeldOD = LoadMaterialDefault();
    sustainMatMiss = LoadMaterialDefault();
    sustainMat.maps[MATERIAL_MAP_DIFFUSE].texture = sustainTexture;
    sustainMat.maps[MATERIAL_MAP_DIFFUSE].color = ColorTint(playerAssets.accentColor, { 180,180,180,255 });
    sustainMatHeld.maps[MATERIAL_MAP_EMISSION].texture = sustainTexture;
    sustainMatHeld.maps[MATERIAL_MAP_EMISSION].color = WHITE;
    sustainMatHeld.maps[MATERIAL_MAP_EMISSION].value = 1;
    sustainMatHeld.maps[MATERIAL_MAP_DIFFUSE].texture = sustainTexture;
    sustainMatHeld.maps[MATERIAL_MAP_DIFFUSE].color = playerAssets.accentColor;
    sustainMatOD.maps[MATERIAL_MAP_DIFFUSE].texture = sustainTexture;
    sustainMatOD.maps[MATERIAL_MAP_DIFFUSE].color = { 180,180,180,255 };
    sustainMatHeldOD.maps[MATERIAL_MAP_DIFFUSE].texture = sustainTexture;
    sustainMatHeldOD.maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    sustainMatMiss.maps[MATERIAL_MAP_DIFFUSE].texture = sustainTexture;
    sustainMatMiss.maps[MATERIAL_MAP_DIFFUSE].color = DARKGRAY;
}

