/*
 * RiceINI.hpp
 *
 *  Created on: Nov 25, 2014
 *      Author: Travis
 */

#ifndef RICEINI_HPP_
#define RICEINI_HPP_

#include <QObject>

#include <stdio.h>


class INIEntry : public QObject
{
    friend class RiceINI;

    Q_OBJECT

    Q_PROPERTY(QString Name READ Name WRITE setName NOTIFY entryUpdated)

    Q_PROPERTY(quint32 NormalCombiner READ NormalCombiner WRITE setNormalCombiner NOTIFY entryUpdated)
    Q_PROPERTY(quint32 NormalBlender READ NormalBlender WRITE setNormalBlender NOTIFY entryUpdated)
    Q_PROPERTY(quint32 FastTextureCRC READ FastTextureCRC WRITE setFastTextureCRC NOTIFY entryUpdated)
    Q_PROPERTY(quint32 AccurateTextureMapping READ AccurateTextureMapping WRITE setAccurateTextureMapping NOTIFY entryUpdated)
    Q_PROPERTY(quint32 FrameBufferOption READ FrameBufferOption WRITE setFrameBufferOption NOTIFY entryUpdated)
    Q_PROPERTY(quint32 RenderToTextureOption READ RenderToTextureOption WRITE setRenderToTextureOption NOTIFY entryUpdated)
    Q_PROPERTY(quint32 ScreenUpdateSetting READ ScreenUpdateSetting WRITE setScreenUpdateSetting NOTIFY entryUpdated)
    Q_PROPERTY(quint32 UseCIWidthAndRatio READ UseCIWidthAndRatio WRITE setUseCIWidthAndRatio NOTIFY entryUpdated)
    Q_PROPERTY(quint32 FullTMEM READ FullTMEM WRITE setFullTMEM NOTIFY entryUpdated)

    Q_PROPERTY(bool DisableBlender READ DisableBlender WRITE setDisableBlender NOTIFY entryUpdated)
    Q_PROPERTY(bool ForceScreenClear READ ForceScreenClear WRITE setForceScreenClear NOTIFY entryUpdated)
    Q_PROPERTY(bool EmulateClear READ EmulateClear WRITE setEmulateClear NOTIFY entryUpdated)
    Q_PROPERTY(bool ForceDepthBuffer READ ForceDepthBuffer WRITE setForceDepthBuffer NOTIFY entryUpdated)
    Q_PROPERTY(bool DisableObjBG READ DisableObjBG WRITE setDisableObjBG NOTIFY entryUpdated)
    Q_PROPERTY(bool DisableTextureCRC READ DisableTextureCRC WRITE setDisableTextureCRC NOTIFY entryUpdated)
    Q_PROPERTY(bool IncTexRectEdge READ IncTexRectEdge WRITE setIncTexRectEdge NOTIFY entryUpdated)
    Q_PROPERTY(bool ZHack READ ZHack WRITE setZHack NOTIFY entryUpdated)
    Q_PROPERTY(bool TextureScaleHack READ TextureScaleHack WRITE setTextureScaleHack NOTIFY entryUpdated)
    Q_PROPERTY(bool FastLoadTile READ FastLoadTile WRITE setFastLoadTile NOTIFY entryUpdated)
    Q_PROPERTY(bool UseSmallerTexture READ UseSmallerTexture WRITE setUseSmallerTexture NOTIFY entryUpdated)
    Q_PROPERTY(bool PrimaryDepthHack READ PrimaryDepthHack WRITE setPrimaryDepthHack NOTIFY entryUpdated)
    Q_PROPERTY(bool Texture1Hack READ Texture1Hack WRITE setTexture1Hack NOTIFY entryUpdated)
    Q_PROPERTY(bool DisableCulling READ DisableCulling WRITE setDisableCulling NOTIFY entryUpdated)
    Q_PROPERTY(bool TxtSizeMethod2 READ TxtSizeMethod2 WRITE setTxtSizeMethod2 NOTIFY entryUpdated)
    Q_PROPERTY(bool EnableTxtLOD READ EnableTxtLOD WRITE setEnableTxtLOD NOTIFY entryUpdated)

    Q_PROPERTY(int VIWidth READ VIWidth WRITE setVIWidth NOTIFY entryUpdated)
    Q_PROPERTY(int VIHeight READ VIHeight WRITE setVIHeight NOTIFY entryUpdated)

signals:
    void entryUpdated();
    void entryChanged();

public:
    enum {
        NOT_USE_CI_WIDTH_AND_RATIO,
        USE_CI_WIDTH_AND_RATIO_FOR_NTSC,
        USE_CI_WIDTH_AND_RATIO_FOR_PAL,
    };

public:
    INIEntry();
    INIEntry(const INIEntry& toCopy);
    virtual ~INIEntry() { }
    INIEntry& operator=(const INIEntry& toCopy);

public:
    inline quint32 NormalCombiner() const { return dwNormalCombiner; }
    inline quint32 NormalBlender() const { return dwNormalBlender; }
    inline quint32 FastTextureCRC() const { return dwFastTextureCRC; }
    inline quint32 AccurateTextureMapping() const { return dwAccurateTextureMapping; }
    inline quint32 FrameBufferOption() const { return dwFrameBufferOption; }
    inline quint32 RenderToTextureOption() const { return dwRenderToTextureOption; }
    inline quint32 ScreenUpdateSetting() const { return dwScreenUpdateSetting; }
    inline quint32 UseCIWidthAndRatio() const { return dwUseCIWidthAndRatio; }
    inline quint32 FullTMEM() const { return dwFullTMEM; }

    inline bool DisableBlender() const { return bDisableBlender; }
    inline bool ForceScreenClear() const { return bForceScreenClear; }
    inline bool EmulateClear() const { return bEmulateClear; }
    inline bool ForceDepthBuffer() const { return bForceDepthBuffer; }
    inline bool DisableObjBG() const { return bDisableObjBG; }
    inline bool DisableTextureCRC() const { return bDisableTextureCRC; }
    inline bool IncTexRectEdge() const { return bIncTexRectEdge; }
    inline bool ZHack() const { return bZHack; }
    inline bool TextureScaleHack() const { return bTextureScaleHack; }
    inline bool FastLoadTile() const { return bFastLoadTile; }
    inline bool UseSmallerTexture() const { return bUseSmallerTexture; }
    inline bool PrimaryDepthHack() const { return bPrimaryDepthHack; }
    inline bool Texture1Hack() const { return bTexture1Hack; }
    inline bool DisableCulling() const { return bDisableCulling; }
    inline bool TxtSizeMethod2() const { return bTxtSizeMethod2; }
    inline bool EnableTxtLOD() const { return bEnableTxtLOD; }

    inline int VIWidth() const { return dwVIWidth; }
    inline int VIHeight() const { return dwVIHeight; }

    inline QString Name() const { return name; }

    inline void setNormalCombiner(quint32 value) { dwNormalCombiner = value; emit entryChanged(); }
    inline void setNormalBlender(quint32 value) { dwNormalBlender = value; emit entryChanged(); }
    inline void setFastTextureCRC(quint32 value) { dwFastTextureCRC = value; emit entryChanged(); }
    inline void setAccurateTextureMapping(quint32 value) { dwAccurateTextureMapping = value; emit entryChanged(); }
    inline void setFrameBufferOption(quint32 value) { dwFrameBufferOption = value; emit entryChanged(); }
    inline void setRenderToTextureOption(quint32 value) { dwRenderToTextureOption = value; emit entryChanged(); }
    inline void setScreenUpdateSetting(quint32 value) { dwScreenUpdateSetting = value; emit entryChanged(); }
    inline void setUseCIWidthAndRatio(quint32 value) { dwUseCIWidthAndRatio = value; emit entryChanged(); }
    inline void setFullTMEM(quint32 value) { dwFullTMEM = value; emit entryChanged(); }

    inline void setDisableBlender(bool value) { bDisableBlender = value; emit entryChanged(); }
    inline void setForceScreenClear(bool value) { bForceScreenClear = value; emit entryChanged(); }
    inline void setEmulateClear(bool value) { bEmulateClear = value; emit entryChanged(); }
    inline void setForceDepthBuffer(bool value) { bForceDepthBuffer = value; emit entryChanged(); }
    inline void setDisableObjBG(bool value) { bDisableObjBG = value; emit entryChanged(); }
    inline void setDisableTextureCRC(bool value) { bDisableTextureCRC = value; emit entryChanged(); }
    inline void setIncTexRectEdge(bool value) { bIncTexRectEdge = value; emit entryChanged(); }
    inline void setZHack(bool value) { bZHack = value; emit entryChanged(); }
    inline void setTextureScaleHack(bool value) { bTextureScaleHack = value; emit entryChanged(); }
    inline void setFastLoadTile(bool value) { bFastLoadTile = value; emit entryChanged(); }
    inline void setUseSmallerTexture(bool value) { bUseSmallerTexture = value; emit entryChanged(); }
    inline void setPrimaryDepthHack(bool value) { bPrimaryDepthHack = value; emit entryChanged(); }
    inline void setTexture1Hack(bool value) { bTexture1Hack = value; emit entryChanged(); }
    inline void setDisableCulling(bool value) { bDisableCulling = value; emit entryChanged(); }
    inline void setTxtSizeMethod2(bool value) { bTxtSizeMethod2 = value; emit entryChanged(); }
    inline void setEnableTxtLOD(bool value) { bEnableTxtLOD = value; emit entryChanged(); }

    inline void setVIWidth(int value) { dwVIWidth = value; emit entryChanged(); }
    inline void setVIHeight(int value) { dwVIHeight = value; emit entryChanged(); }

    inline void setName(const QString& value) { name = QString(value); }

private:
    bool    bOutput;
    QString    crccheck;
    QString    name;

    quint32  dwNormalCombiner;
    quint32  dwNormalBlender;
    quint32  dwFastTextureCRC;
    quint32  dwAccurateTextureMapping;
    quint32  dwFrameBufferOption;
    quint32  dwRenderToTextureOption;
    quint32  dwScreenUpdateSetting;

    bool    bDisableBlender;
    bool    bForceScreenClear;
    bool    bEmulateClear;
    bool    bForceDepthBuffer;

    bool    bDisableObjBG;
    bool    bDisableTextureCRC;
    bool    bIncTexRectEdge;
    bool    bZHack;
    bool    bTextureScaleHack;
    bool    bFastLoadTile;
    bool    bUseSmallerTexture;
    bool    bPrimaryDepthHack;
    bool    bTexture1Hack;
    bool    bDisableCulling;

    int     dwVIWidth;
    int     dwVIHeight;
    quint32  dwUseCIWidthAndRatio;

    quint32  dwFullTMEM;
    bool    bTxtSizeMethod2;
    bool    bEnableTxtLOD;
};

class RiceINI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(INIEntry* CurrentRiceINI READ CurrentRiceINI NOTIFY CurrentFileChanged)

public:
    RiceINI();
    RiceINI(const RiceINI& toCopy);
    virtual ~RiceINI() { }
    RiceINI& operator=(const RiceINI& toCopy);

    void ReadIniFile();
    int WriteIniFile();
    void setCRC(const QString& crc, const QString& name);

signals:
    void CurrentFileChanged();

protected Q_SLOTS:
    void iniFileChanged() { bIniIsChanged = true; }

public:
    inline INIEntry* CurrentRiceINI() { return &m_entries[m_currentIndex]; }

protected:
    void OutputSectionDetails(qint32 i, FILE* fh);

private:
    QList<INIEntry> m_entries;
    int m_currentIndex;
    bool bIniIsChanged;
};

Q_DECLARE_METATYPE(INIEntry)
Q_DECLARE_METATYPE(RiceINI)

#endif /* RICEINI_HPP_ */
