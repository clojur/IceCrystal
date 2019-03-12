////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


//#undef NS_MINIMUM_LOG_LEVEL
//#define NS_MINIMUM_LOG_LEVEL 0


#include "TSF.h"

#include <NsGui/TextBox.h>
#include <NsGui/PasswordBox.h>
#include <NsGui/CompositionUnderline.h>
#include <NsGui/ScrollViewer.h>
#include <NsGui/Decorator.h>
#include <NsGui/Keyboard.h>
#include <NsGui/IntegrationAPI.h>
#include <NsCore/UTF8.h>
#include <NsCore/Log.h>
#include <NsCore/FixedVector.h>
#include <NsCore/Vector.h>
#include <NsCore/Delegate.h>
#include <NsDrawing/Rect.h>

#include <msctf.h>
#include <ctffunc.h>


#define VIEW_COOKIE 0

#define V(x) \
    NS_MACRO_BEGIN \
        HRESULT hr = (x); \
        NS_ASSERT(SUCCEEDED(hr)); \
    NS_MACRO_END


using namespace Noesis;
using namespace NoesisApp;


namespace
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper class to manage COM pointers lifetime
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class ComPtr
{
public:
    ComPtr(): mPtr(0) { }
    ComPtr(T* ptr): mPtr(ptr) { if (mPtr != 0) { mPtr->AddRef(); } }
    ~ComPtr() { if (mPtr != 0) { mPtr->Release(); } }

    operator T*() const { return mPtr; }
    T* operator ->() const { NS_ASSERT(mPtr != 0); return mPtr; }
    T*& Ref() { return mPtr; }

private:
    T* mPtr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
void CopyUTF16(NsFixedVector<char, 256>& dest, const WCHAR* text, ULONG count)
{
    NsFixedVector<WCHAR, 256> source;
    source.resize(count + 1);
    wcsncpy_s(source.data(), source.size(), text, count);
    source[count] = L'\0';

    uint32_t numChars = UTF8::UTF16To8((const uint16_t*)source.data(), dest.data(), 256);
    if (numChars > 256)
    {
        dest.reserve(numChars);
        UTF8::UTF16To8((const uint16_t*)source.data(), dest.data(), numChars);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation of Text Framework Services text store ACP interface
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsfTextStore: public ITextStoreACP, public ITfContextOwnerCompositionSink,
    public ITfTextEditSink
{
public:
    TsfTextStore(): mRefs(1), mSink(0), mCategoryMgr(0), mDisplayAttributeMgr(0), mWindow(0),
        mLockType(0), mTextLength(0), mReconversionNeeded(false)
    {
        V(CoCreateInstance(CLSID_TF_CategoryMgr, 0, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr,
            (void**)&mCategoryMgr));

        V(CoCreateInstance(CLSID_TF_DisplayAttributeMgr, 0, CLSCTX_INPROC_SERVER,
            IID_ITfDisplayAttributeMgr, (void**)&mDisplayAttributeMgr));
    }

    virtual ~TsfTextStore() = 0
    {
        if (mDisplayAttributeMgr != 0)
        {
            mDisplayAttributeMgr->Release();
            mDisplayAttributeMgr = 0;
        }

        if (mCategoryMgr != 0)
        {
            mCategoryMgr->Release();
            mCategoryMgr = 0;
        }
    }

    bool IsEnabled() const
    {
        return mWindow != 0;
    }

    void OnTextChanged(uint32_t start, uint32_t removed, uint32_t added)
    {
        if (mSink != 0 && !HasReadLock() && !HasWriteLock())
        {
            NS_LOG_TRACE("~> OnTextChanged(start=%u, removed=%u, added=%u", start, removed, added);

            TS_TEXTCHANGE change = { LONG(start), LONG(start + removed), LONG(start + added) };
            mSink->OnTextChange(0, &change);
        }
    }

    void OnSelectionChanged()
    {
        if (mSink != 0 && !HasReadLock() && !HasWriteLock())
        {
            NS_LOG_TRACE("~> OnSelectionChanged()");

            mSink->OnSelectionChange();
        }
    }

    void OnLayoutChanged()
    {
        if (mSink != 0 && !HasReadLock() && !HasWriteLock())
        {
            NS_LOG_TRACE("~> OnLayoutChanged()");

            mSink->OnLayoutChange(TS_LC_CHANGE, VIEW_COOKIE);
        }
    }


    // From IUnknown
    //@{
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override
    {
        *ppvObj = NULL;

        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITextStoreACP))
        {
            *ppvObj = static_cast<ITextStoreACP*>(this);
        }
        else if (IsEqualIID(riid, IID_ITfContextOwnerCompositionSink))
        {
            *ppvObj = static_cast<ITfContextOwnerCompositionSink*>(this);
        }
        else if (IsEqualIID(riid, IID_ITfTextEditSink))
        {
            *ppvObj = static_cast<ITfTextEditSink*>(this);
        }

        if (*ppvObj != 0)
        {
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef() override
    {
        return ++mRefs;
    }

    STDMETHODIMP_(ULONG) Release() override
    {
        ULONG newRefs;

        newRefs = --mRefs;

        if (newRefs == 0)
        {
            delete this;
        }

        return newRefs;
    }
    //@}

    // From ITextStoreACP
    //@{
    STDMETHODIMP AdviseSink(REFIID riid, IUnknown* punk, DWORD) override
    {
        if (!IsEqualGUID(riid, IID_ITextStoreACPSink))
        {
            return TS_E_NOOBJECT;
        }

        if (FAILED(punk->QueryInterface<ITextStoreACPSink>(&mSink)))
        {
            return E_NOINTERFACE;
        }

        return S_OK;
    }

    STDMETHODIMP UnadviseSink(IUnknown*) override
    {
        // we're dealing with TSF. We don't have to check punk is same instance of mSink.
        if (mSink != 0)
        {
            mSink->Release();
            mSink = NULL;
        }

        return S_OK;
    }

    STDMETHODIMP RequestLock(DWORD dwLockFlags, HRESULT* phrSession) override
    {
        if (mSink == 0 || !IsEnabled())
        {
            return E_FAIL;
        }

        if (phrSession == 0)
        {
            return E_INVALIDARG;
        }

        NS_LOG_TRACE("RequestLock(lockFlags=%ld)", dwLockFlags);

        if (mLockType != 0)
        {
            if ((dwLockFlags & TS_LF_SYNC) != 0)
            {
                NS_LOG_TRACE("--> Error: Already locked");

                // Can't lock synchronously
                *phrSession = TS_E_SYNCHRONOUS;
                return S_OK;
            }
            else
            {
                NS_LOG_TRACE("--> Queue lock");

                // Queue an asynchronous lock
                mLockQueue.push_back(dwLockFlags & TS_LF_READWRITE);
                *phrSession = TS_S_ASYNC;
                return S_OK;
            }
        }

        // Lock
        NS_LOG_TRACE("--> Lock Begin");
        mLockType = dwLockFlags & TS_LF_READWRITE;
        *phrSession = mSink->OnLockGranted(mLockType);
        mLockType = 0;
        NS_LOG_TRACE("--> Lock End");

        // Handle pending lock requests
        while (!mLockQueue.empty())
        {
            NS_LOG_TRACE("RequestLock(lockFlags=%ld) [queued]", mLockQueue.front());

            NS_LOG_TRACE("--> Lock Begin");
            mLockType = mLockQueue.front();
            mLockQueue.erase(mLockQueue.begin());
            mSink->OnLockGranted(mLockType);
            mLockType = 0;
            NS_LOG_TRACE("--> Lock End");
        }

        return S_OK;
    }

    STDMETHODIMP GetStatus(TS_STATUS* pdcs) override
    {
        if (pdcs == 0)
        {
            return E_INVALIDARG;
        }

        // We use transitory contexts and we don't support hidden text
        pdcs->dwDynamicFlags = 0;
        pdcs->dwStaticFlags = TS_SS_TRANSITORY | TS_SS_NOHIDDENTEXT;

        return S_OK;
    }

    STDMETHODIMP QueryInsert(LONG acpInsertStart, LONG acpInsertEnd, ULONG cch,
        LONG* pacpResultStart, LONG* pacpResultEnd) override
    {
        if (pacpResultStart == 0 || pacpResultEnd == 0)
        {
            return E_INVALIDARG;
        }

        NS_LOG_TRACE("QueryInsert(acpStart=%ld, acpEnd=%ld, cch=%lu)", acpInsertStart,
            acpInsertEnd, cch);

        *pacpResultStart = acpInsertStart;
        *pacpResultEnd = acpInsertStart + cch;

        return S_OK;
    }

    STDMETHODIMP GetSelection(ULONG ulIndex, ULONG ulCount, TS_SELECTION_ACP* pSelection,
        ULONG* pcFetched) override
    {
        if (ulCount == 0 || pcFetched == 0)
        {
            return E_INVALIDARG;
        }
        if (!HasReadLock())
        {
            return TS_E_NOLOCK;
        }

        NS_LOG_TRACE("GetSelection(ulIndex=%lu, ulCount=%lu)", ulIndex, ulCount);

        *pcFetched = 0;
        if (ulCount > 0 && (ulIndex == 0 || ulIndex == TS_DEFAULT_SELECTION))
        {
            pSelection[0].acpStart = GetSelectionStart();
            pSelection[0].acpEnd = GetSelectionLength() + pSelection[0].acpStart;
            pSelection[0].style.ase = TS_AE_END;
            pSelection[0].style.fInterimChar = FALSE;
            *pcFetched = 1;

            NS_LOG_TRACE("--> acpStart=%ld, acpEnd=%ld", pSelection[0].acpStart,
                pSelection[0].acpEnd);
        }

        return S_OK;
    }

    STDMETHODIMP SetSelection(ULONG ulCount, const TS_SELECTION_ACP* pSelection) override
    {
        if (!HasWriteLock())
        {
            return TF_E_NOLOCK;
        }

        NS_LOG_TRACE("SetSelection(ulCount=%lu, Sel.acpStart=%ld, Sel.acpEnd=%ld)", ulCount,
            pSelection[0].acpStart, pSelection[0].acpEnd);

        if (ulCount > 0)
        {
            SetSelectionStart(pSelection[0].acpStart);
            SetSelectionLength(pSelection[0].acpEnd - pSelection[0].acpStart);
        }

        return S_OK;
    }

    STDMETHODIMP GetText(LONG acpStart, LONG acpEnd, __out_ecount(cchPlainReq) WCHAR* pchPlain,
        ULONG cchPlainReq, ULONG* pcchPlainOut, TS_RUNINFO* prgRunInfo, ULONG ulRunInfoReq,
        ULONG* pulRunInfoOut, LONG* pacpNext) override
    {
        if (pcchPlainOut == 0 || pulRunInfoOut == 0 || pacpNext == 0)
        {
            return E_INVALIDARG;
        }
        if (pchPlain == 0 && cchPlainReq != 0)
        {
            return E_INVALIDARG;
        }
        if (prgRunInfo == 0 && ulRunInfoReq != 0)
        {
            return E_INVALIDARG;
        }
        if (acpStart < 0 || acpEnd < -1)
        {
            return E_INVALIDARG;
        }
        if (!HasReadLock())
        {
            return TF_E_NOLOCK;
        }

        *((uint16_t*)pchPlain) = 0;

        if (cchPlainReq == 0 && ulRunInfoReq == 0)
        {
            return S_OK;
        }

        NS_LOG_TRACE("GetText(acpStart=%ld, acpEnd=%ld, cchPlainReq=%lu)",
            acpStart, acpEnd, cchPlainReq);

        const char* text = GetText();
        uint32_t textLength = GetTextLength();

        if (acpEnd == -1)
        {
            acpEnd = textLength;
        }

        acpEnd = Min(acpEnd, acpStart + (int)cchPlainReq);

        if (acpStart < acpEnd)
        {
            UTF8::Advance(text, acpStart);
            UTF8::UTF8To16(text, (uint16_t*)pchPlain, acpEnd - acpStart + 1);
        }

        NS_LOG_TRACE("--> text=\"%s\", cch=%ld", text, acpEnd - acpStart);

        *pcchPlainOut = acpEnd - acpStart;

        if (ulRunInfoReq != 0)
        {
            prgRunInfo[0].uCount = acpEnd - acpStart;
            prgRunInfo[0].type = TS_RT_PLAIN;
            *pulRunInfoOut = 1;
        }

        *pacpNext = acpEnd;

        return S_OK;
    }

    STDMETHODIMP SetText(DWORD dwFlags, LONG acpStart, LONG acpEnd,
        __in_ecount(cch) const WCHAR* pchText, ULONG cch, TS_TEXTCHANGE* pChange) override
    {
        if (acpStart < 0 || acpEnd < 0)
        {
            return E_INVALIDARG;
        }
        if (!HasWriteLock())
        {
            return TF_E_NOLOCK;
        }

        int textLength = GetTextLength();
        if (acpStart > textLength)
        {
            return E_INVALIDARG;
        }

        NS_LOG_TRACE("SetText(flags=%ld, acpStart=%ld, acpEnd=%ld)", dwFlags, acpStart, acpEnd);

        // A reconversion can be initiated when user presses the Space key and some text is,
        // selected, in that case the conversion candidate list is shown and we are finished here
        if (TryReconversion(pchText, cch))
        {
            return S_OK;
        }

        SetSelectionStart(acpStart);
        SetSelectionLength(acpEnd - acpStart);

        NsFixedVector<char, 256> buffer;
        CopyUTF16(buffer, pchText, cch);

        NS_LOG_TRACE("--> text=\"%s\", cch=%lu", buffer.data(), cch);

        SetSelectedText(buffer.data());

        if (pChange != 0)
        {
            pChange->acpStart = acpStart;
            pChange->acpOldEnd = acpEnd;
            pChange->acpNewEnd = acpStart + cch;
        }

        return S_OK;
    }

    STDMETHODIMP GetFormattedText(LONG, LONG, IDataObject**) override
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP GetEmbedded(LONG, REFGUID, REFIID, IUnknown**) override
    {
        // We don't support any embedded objects.
        return E_NOTIMPL;
    }

    STDMETHODIMP InsertEmbedded(DWORD, LONG, LONG, IDataObject*, TS_TEXTCHANGE*) override
    {
        // We don't support any embedded objects.
        return E_NOTIMPL;
    }

    STDMETHODIMP RequestSupportedAttrs(DWORD, ULONG, const TS_ATTRID* paFilterAttrs) override
    {
        if (paFilterAttrs == 0)
        {
            return E_INVALIDARG;
        }

        // We don't support attributes
        return E_FAIL;
    }

    STDMETHODIMP RequestAttrsAtPosition(LONG, ULONG, const TS_ATTRID*, DWORD) override
    {
        // We don't support any document attributes. This method just returns S_OK, and subsequently
        // called RetrieveRequestedAttrs() returns 0 as the number of supported attributes
        return S_OK;
    }

    STDMETHODIMP RequestAttrsTransitioningAtPosition(LONG, ULONG, const TS_ATTRID*, DWORD) override
    {
        // We don't support any document attributes. This method just returns S_OK, and subsequently
        // called RetrieveRequestedAttrs() returns 0 as the number of supported attributes
        return S_OK;
    }

    STDMETHODIMP FindNextAttrTransition(LONG, LONG, ULONG, const TS_ATTRID*, DWORD,
        LONG* pacpNext, BOOL* pfFound, LONG* plFoundOffset) override
    {
        if (pacpNext == 0 || pfFound == 0 || plFoundOffset == 0)
        {
            return E_INVALIDARG;
        }

        // We don't support any attributes, so we always return "not found"
        *pacpNext = 0;
        *pfFound = FALSE;
        *plFoundOffset = 0;

        return S_OK;
    }

    STDMETHODIMP RetrieveRequestedAttrs(ULONG, TS_ATTRVAL* paAttrVals, ULONG* pcFetched) override
    {
        if (paAttrVals == 0 || pcFetched == 0)
        {
            return E_INVALIDARG;
        }

        *pcFetched = 0;
        return S_OK;
    }

    STDMETHODIMP GetEndACP(LONG* pacp) override
    {
        if (pacp == 0)
        {
            return E_INVALIDARG;
        }
        if (!HasReadLock())
        {
            return TS_E_NOLOCK;
        }

        *pacp = (LONG)GetTextLength();

        return S_OK;
    }

    STDMETHODIMP GetActiveView(TsViewCookie* pvcView) override
    {
        // We support only one view
        *pvcView = VIEW_COOKIE;
        return S_OK;
    }

    STDMETHODIMP GetACPFromPoint(TsViewCookie, const POINT*, DWORD, LONG*) override
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP GetTextExt(TsViewCookie vcView, LONG acpStart, LONG acpEnd, RECT* prc,
        BOOL* pfClipped) override
    {
        if (prc == 0 || pfClipped == 0 || vcView != VIEW_COOKIE)
        {
            return E_INVALIDARG;
        }
        if (acpStart < 0 || acpEnd < 0)
        {
            return E_INVALIDARG;
        }
        if (!HasReadLock())
        {
            return TF_E_NOLOCK;
        }

        NS_LOG_TRACE("GetTextExt(vcView=%ld, acpStart=%ld, acpEnd=%ld)", vcView, acpStart, acpEnd);

        // Make sure control is measured and arranged correctly
        UpdateLayout();

        // Get bounding box of the selected text
        Rect bounds = GetRangeBounds(acpStart, acpEnd);

        // Add some space for the underlines
        float fontSize = GetFontSize();
        bounds.height += Max(1.0f, fontSize * 0.1f);

        // Convert bounding box vertices to client coordinates
        Visual* textPresenter = GetTextPresenter();
        Point topLeft = textPresenter->PointToScreen(bounds.GetTopLeft());
        Point bottomRight = textPresenter->PointToScreen(bounds.GetBottomRight());

        prc->left = (LONG)Round(topLeft.x);
        prc->right = (LONG)Round(bottomRight.x);
        prc->top = (LONG)Round(topLeft.y);
        prc->bottom = (LONG)Round(bottomRight.y);

        ClientToScreen(mWindow, (POINT*)&prc->left);
        ClientToScreen(mWindow, (POINT*)&prc->right);

        NS_LOG_TRACE("--> (%ld, %ld), %ldx%ld",
            prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top);

        *pfClipped = FALSE;

        return S_OK;
    }

    STDMETHODIMP GetScreenExt(TsViewCookie vcView, RECT* prc) override
    {
        if (prc == 0 || vcView != VIEW_COOKIE)
        {
            return E_INVALIDARG;
        }

        NS_LOG_TRACE("GetScreenExt(vcView=%ld)", vcView);

        GetClientRect(mWindow, prc);

        ClientToScreen(mWindow, (POINT*)&prc->left);
        ClientToScreen(mWindow, (POINT*)&prc->right);

        return S_OK;
    }

    STDMETHODIMP GetWnd(TsViewCookie vcView, HWND* phWnd) override
    {
        if (phWnd == 0 || vcView != VIEW_COOKIE)
        {
            return E_INVALIDARG;
        }

        *phWnd = mWindow;

        return S_OK;
    }

    STDMETHODIMP QueryInsertEmbedded(const GUID*, const FORMATETC*, BOOL*) override
    {
        // We don't support any embedded objects.
        return E_NOTIMPL;
    }

    STDMETHODIMP InsertTextAtSelection(DWORD dwFlags, __in_ecount(cch) const WCHAR* pchText,
        ULONG cch, LONG* pacpStart, LONG* pacpEnd, TS_TEXTCHANGE* pChange) override
    {
        NS_LOG_TRACE("InsertTextAtSelection(flags=%ld)", dwFlags);

        LONG acpStart = (LONG)GetSelectionStart();
        LONG acpEnd = (LONG)GetSelectionLength() + acpStart;

        if ((dwFlags & TS_IAS_QUERYONLY) != 0)
        {
            if (!HasReadLock())
            {
                return TF_E_NOLOCK;
            }

            if (pacpStart != 0)
            {
                *pacpStart = acpStart;
            }
            if (pacpEnd != 0)
            {
                *pacpEnd = acpEnd;
            }

            NS_LOG_TRACE("--> QueryOnly: acpStart=%ld, acpEnd=%ld", acpStart, acpEnd);

            return S_OK;
        }

        if (pchText == 0)
        {
            return E_INVALIDARG;
        }
        if (!HasWriteLock())
        {
            return TS_E_NOLOCK;
        }

        if (pchText != 0 && cch > 0)
        {
            NsFixedVector<char, 256> buffer;
            CopyUTF16(buffer, pchText, cch);

            SetSelectedText(buffer.data());

            NS_LOG_TRACE("--> text=\"%s\", cch=%lu", buffer.data(), cch);
        }

        if (pacpStart != 0)
        {
            *pacpStart = acpStart;
        }
        if (pacpEnd != 0)
        {
            *pacpEnd = acpStart + cch;
        }
        if (pChange != 0)
        {
            pChange->acpStart = acpStart;
            pChange->acpOldEnd = acpEnd;
            pChange->acpNewEnd = acpStart + cch;
        }

        SetSelectionStart(acpStart);
        SetSelectionLength(cch);

        NS_LOG_TRACE("--> acpStart=%ld, acpEnd=%ld", acpStart, acpStart + cch);

        return S_OK;
    }

    STDMETHODIMP InsertEmbeddedAtSelection(DWORD, IDataObject*, LONG*, LONG*,
        TS_TEXTCHANGE*) override
    {
        // We don't support any embedded objects.
        return E_NOTIMPL;
    }
    //@}

    // From ITfContextOwnerCompositionSink
    //@{
    STDMETHODIMP OnStartComposition(ITfCompositionView* pComposition, BOOL* pfOk) override
    {
        NS_LOG_TRACE("OnStartComposition(composition=%p)", pComposition);

        if (pfOk != 0)
        {
            *pfOk = TRUE;
        }

        return S_OK;
    }

    STDMETHODIMP OnUpdateComposition(ITfCompositionView * pComposition, ITfRange* pRange) override
    {
        NS_LOG_TRACE("OnUpdateComposition(composition=%p, range=%p)", pComposition, pRange);
        return S_OK;
    }

    STDMETHODIMP OnEndComposition(ITfCompositionView* pComposition) override
    {
        NS_LOG_TRACE("OnEndComposition(composition=%p)", pComposition);
        return S_OK;
    }
    //@}

    // From ITfTextEditSink
    //@{
    STDMETHODIMP OnEndEdit(ITfContext* context, TfEditCookie ecReadOnly,
        ITfEditRecord* pEditRecord) override
    {
        if (context == 0 || pEditRecord == 0)
        {
            return E_INVALIDARG;
        }

        if (!HasWriteLock())
        {
            // nothing modified
            return S_OK;
        }

        NS_LOG_TRACE("OnEndEdit(context=%p, editCookie=%ld, editRecor=%p)", context, ecReadOnly,
            pEditRecord);

        const GUID* guids[2] = { &GUID_PROP_COMPOSING, &GUID_PROP_ATTRIBUTE };
        ComPtr<ITfReadOnlyProperty> trackProperty;
        V(context->TrackProperties(guids, 2, NULL, 0, &trackProperty.Ref()));

        ClearCompositionUnderlines();

        ComPtr<ITfRange> startEndRange;
        ComPtr<ITfRange> endRange;
        V(context->GetStart(ecReadOnly, &startEndRange.Ref()));
        V(context->GetEnd(ecReadOnly, &endRange.Ref()));
        V(startEndRange->ShiftEndToRange(ecReadOnly, endRange, TF_ANCHOR_END));

        ComPtr<IEnumTfRanges> ranges;
        V(trackProperty->EnumRanges(ecReadOnly, &ranges.Ref(), startEndRange));

        for (;;)
        {
            ULONG fetched;
            ComPtr<ITfRange> range;
            if (ranges->Next(1, &range.Ref(), &fetched) != S_OK)
            {
                break;
            }

            VARIANT value;
            ComPtr<IEnumTfPropertyValue> enumPropertyValue;
            V(trackProperty->GetValue(ecReadOnly, range, &value));
            NS_ASSERT(value.punkVal != 0);
            V(value.punkVal->QueryInterface<IEnumTfPropertyValue>(&enumPropertyValue.Ref()));

            bool isComposing = false;
            bool hasDisplayAttribute = false;
            TF_PROPERTYVAL propertyValue = TF_PROPERTYVAL();
            TF_DISPLAYATTRIBUTE displayAttribute = TF_DISPLAYATTRIBUTE();
            while (enumPropertyValue->Next(1, &propertyValue, &fetched) == S_OK)
            {
                if (IsEqualGUID(propertyValue.guidId, GUID_PROP_COMPOSING))
                {
                    isComposing = propertyValue.varValue.lVal == TRUE;
                }
                else if (IsEqualGUID(propertyValue.guidId, GUID_PROP_ATTRIBUTE))
                {
                    TfGuidAtom atom = static_cast<TfGuidAtom>(propertyValue.varValue.lVal);
                    hasDisplayAttribute = GetDisplayAttribute(atom, &displayAttribute);
                }
                VariantClear(&propertyValue.varValue);
            }

            ComPtr<ITfRangeACP> acpRange;
            V(range->QueryInterface<ITfRangeACP>(&acpRange.Ref()));

            LONG start, length;
            V(acpRange->GetExtent(&start, &length));
            if (isComposing)
            {
                CompositionLineStyle style = (CompositionLineStyle)displayAttribute.lsStyle;
                CompositionUnderline underline = { uint32_t(start), uint32_t(start + length), style,
                    displayAttribute.fBoldLine == TRUE ? true : false };
                AddCompositionUnderline(underline);

                NS_LOG_TRACE("--> Underline: %u-%u, %d, %s", underline.start, underline.end,
                    (int)style, underline.bold ? "Bold" : "Normal");
            }

            VariantClear(&value);
        }

        if (mReconversionNeeded)
        {
            mReconversionNeeded = false;
            DoReconversion();
        }

        return S_OK;
    }
    //@}

protected:
    void AssociateWindow(HWND hWnd)
    {
        mWindow = hWnd;
    }

    virtual int32_t GetSelectionStart() const = 0;
    virtual void SetSelectionStart(int32_t start) = 0;
    virtual int32_t GetSelectionLength() const = 0;
    virtual void SetSelectionLength(int32_t len) = 0;
    virtual void SetSelectedText(const char* text) = 0;
    virtual const char* GetText() const = 0;
    virtual uint32_t GetTextLength() const = 0;
    virtual Rect GetRangeBounds(uint32_t start, uint32_t end) const = 0;
    virtual float GetFontSize() const = 0;
    virtual void UpdateLayout() = 0;
    virtual Visual* GetTextPresenter() const = 0;
    virtual void AddCompositionUnderline(const CompositionUnderline& underline) const = 0;
    virtual void ClearCompositionUnderlines() const = 0;
    virtual bool TryReconversion() = 0;
    virtual void DoReconversion() = 0;

private:
    bool HasReadLock() const
    {
        return (mLockType & TS_LF_READ) == TS_LF_READ;
    }

    bool HasWriteLock() const
    {
        return (mLockType & TS_LF_READWRITE) == TS_LF_READWRITE;
    }

    bool IsSpaceChar(WCHAR ch)
    {
        const WCHAR halfWidthSpace = 0x0020;
        const WCHAR fullWidthSpace = 0x3000;
        return ch == halfWidthSpace || ch == fullWidthSpace;
    }

    bool TryReconversion(const WCHAR* text, ULONG len)
    {
        if (GetSelectionLength() > 0)
        {
            if ((len == 0 || len == 1) && IsSpaceChar(*text))
            {
                if (TryReconversion())
                {
                    mReconversionNeeded = true;
                    return true;
                }
            }
        }

        return false;
    }

    bool GetDisplayAttribute(TfGuidAtom atom, TF_DISPLAYATTRIBUTE* attribute) const
    {
        GUID guid;
        if (FAILED(mCategoryMgr->GetGUID(atom, &guid)))
        {
            return false;
        }

        ComPtr<ITfDisplayAttributeInfo> info;
        if (FAILED(mDisplayAttributeMgr->GetDisplayAttributeInfo(guid, &info.Ref(), NULL)))
        {
            return false;
        }

        return SUCCEEDED(info->GetAttributeInfo(attribute));
    }

private:
    ULONG mRefs;
    ITextStoreACPSink* mSink;
    ITfCategoryMgr* mCategoryMgr;
    ITfDisplayAttributeMgr* mDisplayAttributeMgr;
    HWND mWindow;
    DWORD mLockType;
    uint32_t mTextLength;
    bool mReconversionNeeded;

    NsVector<DWORD> mLockQueue;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsfTextBoxTextStore final: public TsfTextStore
{
public:
    TsfTextBoxTextStore(): mEditContext(0), mEditCookie(TF_INVALID_COOKIE), mReconversion(0),
        mReconversionRange(0), mControl(0), mTextLength(0)
    {
    }

    ~TsfTextBoxTextStore()
    {
        if (mEditContext != 0)
        {
            mEditContext->Release();
            mEditContext = 0;
            mEditCookie = TF_INVALID_COOKIE;
        }

        if (mReconversionRange != 0)
        {
            mReconversionRange->Release();
            mReconversionRange = 0;
        }

        if (mReconversion != 0)
        {
            mReconversion->Release();
            mReconversion = 0;
        }
    }

    void SetEditContext(ITfContext* editContext, TfEditCookie editCookie,
        ITfFnReconversion* reconversion)
    {
        mEditContext = editContext;
        mEditContext->AddRef();

        mEditCookie = editCookie;

        mReconversion = reconversion;
        mReconversion->AddRef();
    }

    void AssociateControl(HWND hWnd, TextBox* control)
    {
        if (mControl != 0)
        {
            UnregisterScrollChanged(NsDynamicCast<ScrollViewer*>(mControl->GetContentHost()));
            UnregisterSelectionChanged();
            UnregisterTextChanged();

            mTextLength = 0;
        }

        AssociateWindow(hWnd);
        mControl = control;

        if (mControl != 0)
        {
            mTextLength = UTF8::Length(GetText());

            RegisterTextChanged();
            RegisterSelectionChanged();
            RegisterScrollChanged(NsDynamicCast<ScrollViewer*>(mControl->GetContentHost()));
        }
    }

    TextBox* GetControl() const
    {
        return mControl;
    }

protected:
    int32_t GetSelectionStart() const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetSelectionStart();
    }

    void SetSelectionStart(int32_t start) override
    {
        NS_ASSERT(mControl != 0);
        mControl->SetSelectionStart(start);
    }

    int32_t GetSelectionLength() const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetSelectionLength();
    }

    void SetSelectionLength(int32_t len) override
    {
        NS_ASSERT(mControl != 0);
        mControl->SetSelectionLength(len);
    }

    void SetSelectedText(const char* text) override
    {
        NS_ASSERT(mControl != 0);
        mControl->SetSelectedText(text);
    }

    const char* GetText() const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetText();
    }

    uint32_t GetTextLength() const
    {
        return mTextLength;
    }

    Rect GetRangeBounds(uint32_t start, uint32_t end) const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetRangeBounds(start, end);
    }

    float GetFontSize() const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetFontSize();
    }

    void UpdateLayout() override
    {
        NS_ASSERT(mControl != 0);
        mControl->UpdateLayout();
    }

    Visual* GetTextPresenter() const override
    {
        UIElement* contentHost = mControl->GetContentHost();

        ScrollViewer* scrollViewer = NsDynamicCast<ScrollViewer*>(contentHost);
        if (scrollViewer != 0)
        {
            return NsStaticCast<Visual*>(scrollViewer->GetContent());
        }

        Decorator* decorator = NsDynamicCast<Decorator*>(contentHost);
        if (decorator != 0)
        {
            return NsStaticCast<Visual*>(decorator->GetChild());
        }

        return mControl;
    }

    void AddCompositionUnderline(const CompositionUnderline& underline) const override
    {
        mControl->AddCompositionUnderline(underline);
    }

    void ClearCompositionUnderlines() const override
    {
        mControl->ClearCompositionUnderlines();
    }

    bool TryReconversion() override
    {
        NS_ASSERT(mEditContext != 0);
        NS_ASSERT(mReconversion != 0);

        NS_LOG_TRACE("TryReconversion()");

        ULONG fetched = 0;
        TF_SELECTION selection = TF_SELECTION();
        V(mEditContext->GetSelection(mEditCookie, TF_DEFAULT_SELECTION, 1, &selection, &fetched));

        NS_ASSERT(fetched > 0);

        BOOL convertable = FALSE;
        ComPtr<ITfRange> range;
        V(mReconversion->QueryRange(selection.range, &range.Ref(), &convertable));

        if (convertable != FALSE && range != 0)
        {
            mReconversionRange = (ITfRange*)range;
            mReconversionRange->AddRef();

            return true;
        }

        return false;
    }

    void DoReconversion() override
    {
        NS_ASSERT(mReconversion != 0);
        NS_ASSERT(mReconversionRange != 0);

        NS_LOG_TRACE("DoReconversion()");

        V(mReconversion->Reconvert(mReconversionRange));

        mReconversionRange->Release();
        mReconversionRange = 0;
    }

private:
    void RegisterTextChanged()
    {
        NS_ASSERT(mControl != 0);
        mControl->TextChanged() += MakeDelegate(this, &TsfTextBoxTextStore::OnControlTextChanged);
    }
    void UnregisterTextChanged()
    {
        NS_ASSERT(mControl != 0);
        mControl->TextChanged() -= MakeDelegate(this, &TsfTextBoxTextStore::OnControlTextChanged);
    }
    void OnControlTextChanged(BaseComponent*, const RoutedEventArgs&)
    {
        uint32_t newLength = UTF8::Length(GetText());
        uint32_t removed = mTextLength > newLength ? mTextLength - newLength : 0;
        uint32_t added = mTextLength < newLength ? newLength - mTextLength : 0;
        mTextLength = newLength;

        OnTextChanged(GetSelectionStart(), removed, added);
    }

    void RegisterSelectionChanged()
    {
        NS_ASSERT(mControl != 0);
        mControl->SelectionChanged() += MakeDelegate(this,
            &TsfTextBoxTextStore::OnControlSelectionChanged);
    }
    void UnregisterSelectionChanged()
    {
        NS_ASSERT(mControl != 0);
        mControl->SelectionChanged() -= MakeDelegate(this,
            &TsfTextBoxTextStore::OnControlSelectionChanged);
    }
    void OnControlSelectionChanged(BaseComponent*, const RoutedEventArgs&)
    {
        OnSelectionChanged();
    }

    void RegisterScrollChanged(ScrollViewer* scrollViewer)
    {
        if (scrollViewer != 0)
        {
            scrollViewer->ScrollChanged() += MakeDelegate(this,
                &TsfTextBoxTextStore::OnScrollChanged);
        }
    }
    void UnregisterScrollChanged(ScrollViewer* scrollViewer)
    {
        if (scrollViewer != 0)
        {
            scrollViewer->ScrollChanged() -= MakeDelegate(this,
                &TsfTextBoxTextStore::OnScrollChanged);
        }
    }
    void OnScrollChanged(BaseComponent*, const ScrollChangedEventArgs&)
    {
        OnLayoutChanged();
    }

private:
    ITfContext* mEditContext;
    TfEditCookie mEditCookie;
    ITfFnReconversion* mReconversion;
    ITfRange* mReconversionRange;
    TextBox* mControl;
    uint32_t mTextLength;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class TsfPassBoxTextStore final: public TsfTextStore
{
public:
    TsfPassBoxTextStore(): mControl(0) { }

    void AssociateControl(HWND hWnd, PasswordBox* control)
    {
        AssociateWindow(hWnd);
        mControl = control;
    }

    PasswordBox* GetControl() const
    {
        return mControl;
    }

protected:
    int32_t GetSelectionStart() const override
    {
        NS_ASSERT(mControl != 0);
        return 0;
    }

    void SetSelectionStart(int32_t) override
    {
        NS_ASSERT(mControl != 0);
    }

    int32_t GetSelectionLength() const override
    {
        NS_ASSERT(mControl != 0);
        return 0;
    }

    void SetSelectionLength(int32_t) override
    {
        NS_ASSERT(mControl != 0);
    }

    void SetSelectedText(const char*) override
    {
        NS_ASSERT(mControl != 0);
    }

    const char* GetText() const override
    {
        NS_ASSERT(mControl != 0);
        return "";
    }

    uint32_t GetTextLength() const
    {
        return 0;
    }

    Rect GetRangeBounds(uint32_t, uint32_t) const override
    {
        NS_ASSERT(mControl != 0);
        return Rect::Empty();
    }

    float GetFontSize() const override
    {
        NS_ASSERT(mControl != 0);
        return mControl->GetFontSize();
    }

    void UpdateLayout() override
    {
        NS_ASSERT(mControl != 0);
        mControl->UpdateLayout();
    }

    Visual* GetTextPresenter() const override
    {
        return mControl;
    }

    void AddCompositionUnderline(const CompositionUnderline&) const override
    {
    }

    void ClearCompositionUnderlines() const override
    {
    }

    bool TryReconversion() override
    {
        return false;
    }

    void DoReconversion() override
    {
    }

private:
    PasswordBox* mControl;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
int gInitCount = 0;
ITfThreadMgr* gThreadMgr = 0;
TfClientId gClientId = TF_CLIENTID_NULL;
ITfDocumentMgr* gTextBoxDocumentMgr = 0;
ITfDocumentMgr* gPassBoxDocumentMgr = 0;
ITfDocumentMgr* gDisabledDocumentMgr = 0;
TsfTextBoxTextStore* gTextBoxStore = 0;
TsfPassBoxTextStore* gPassBoxStore = 0;
DWORD gTextBoxEditCookie = TF_INVALID_COOKIE;
HWND gCurrentWindow = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
void SetCompartmentValue(ITfCompartmentMgr* compartmentMgr, const GUID& key, DWORD value)
{
    ComPtr<ITfCompartment> compartment;
    V(compartmentMgr->GetCompartment(key, &compartment.Ref()));

    VARIANT variant;
    ::VariantInit(&variant);
    variant.vt = VT_I4;
    variant.lVal = value;
    V(compartment->SetValue(gClientId, &variant));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateTextBoxDocument()
{
    V(gThreadMgr->CreateDocumentMgr(&gTextBoxDocumentMgr));

    gTextBoxStore = new TsfTextBoxTextStore();

    TfEditCookie inputCookie;
    ComPtr<ITfContext> inputContext;
    V(gTextBoxDocumentMgr->CreateContext(gClientId, 0, (ITextStoreACP*)gTextBoxStore,
        &inputContext.Ref(), &inputCookie));

    ComPtr<ITfFunctionProvider> functionProvider;
    V(gThreadMgr->GetFunctionProvider(GUID_SYSTEM_FUNCTIONPROVIDER,
        &functionProvider.Ref()));

    ComPtr<ITfFnReconversion> reconversion;
    V(functionProvider->GetFunction(GUID_NULL, IID_ITfFnReconversion,
        (IUnknown**)&reconversion.Ref()));

    gTextBoxStore->SetEditContext(inputContext, inputCookie, reconversion);

    V(gTextBoxDocumentMgr->Push(inputContext));

    ComPtr<ITfSource> source;
    V(inputContext->QueryInterface<ITfSource>(&source.Ref()));

    V(source->AdviseSink(IID_ITfTextEditSink, (ITfTextEditSink*)gTextBoxStore,
        &gTextBoxEditCookie));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CreatePassBoxDocument()
{
    V(gThreadMgr->CreateDocumentMgr(&gPassBoxDocumentMgr));

    gPassBoxStore = new TsfPassBoxTextStore();

    TfEditCookie inputCookie;
    ComPtr<ITfContext> inputContext;
    V(gPassBoxDocumentMgr->CreateContext(gClientId, 0, (ITextStoreACP*)gPassBoxStore,
        &inputContext.Ref(), &inputCookie));

    ComPtr<ITfCompartmentMgr> compartmentMgr;
    V(inputContext->QueryInterface<ITfCompartmentMgr>(&compartmentMgr.Ref()));

    SetCompartmentValue(compartmentMgr, GUID_COMPARTMENT_KEYBOARD_DISABLED, 1);
    SetCompartmentValue(compartmentMgr, GUID_COMPARTMENT_EMPTYCONTEXT, 1);

    V(gPassBoxDocumentMgr->Push(inputContext));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateDisabledDocument()
{
    V(gThreadMgr->CreateDocumentMgr(&gDisabledDocumentMgr));

    TfEditCookie inputCookie;
    ComPtr<ITfContext> inputContext;
    V(gDisabledDocumentMgr->CreateContext(gClientId, 0, 0, &inputContext.Ref(), &inputCookie));

    ComPtr<ITfCompartmentMgr> compartmentMgr;
    V(inputContext->QueryInterface<ITfCompartmentMgr>(&compartmentMgr.Ref()));

    SetCompartmentValue(compartmentMgr, GUID_COMPARTMENT_KEYBOARD_DISABLED, 1);
    SetCompartmentValue(compartmentMgr, GUID_COMPARTMENT_EMPTYCONTEXT, 1);

    V(gDisabledDocumentMgr->Push(inputContext));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Japanese IME expects the default value of this compartment to be TF_SENTENCEMODE_PHRASEPREDICT
// like IMM32 implementation. This value does not affect other language's IME behaviors
void ConfigureJapaneseIME()
{
    ComPtr<ITfCompartmentMgr> compartmentMgr;
    V(gThreadMgr->QueryInterface<ITfCompartmentMgr>(&compartmentMgr.Ref()));

    SetCompartmentValue(compartmentMgr, GUID_COMPARTMENT_KEYBOARD_INPUTMODE_SENTENCE,
        TF_SENTENCEMODE_PHRASEPREDICT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DestroyTextBoxDocument()
{
    if (gTextBoxDocumentMgr != 0)
    {
        ComPtr<ITfContext> inputContext;
        V(gTextBoxDocumentMgr->GetTop(&inputContext.Ref()));

        if (gTextBoxEditCookie != TF_INVALID_COOKIE)
        {
            ComPtr<ITfSource> source;
            V(inputContext->QueryInterface<ITfSource>(&source.Ref()));

            V(source->UnadviseSink(gTextBoxEditCookie));
            gTextBoxEditCookie = TF_INVALID_COOKIE;
        }

        gTextBoxDocumentMgr->Pop(TF_POPF_ALL);
        gTextBoxDocumentMgr->Release();
        gTextBoxDocumentMgr = 0;
    }

    if (gTextBoxStore != 0)
    {
        gTextBoxStore->Release();
        gTextBoxStore = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DestroyPassBoxDocument()
{
    if (gPassBoxDocumentMgr != 0)
    {
        ComPtr<ITfContext> inputContext;
        V(gPassBoxDocumentMgr->GetTop(&inputContext.Ref()));

        gPassBoxDocumentMgr->Pop(TF_POPF_ALL);
        gPassBoxDocumentMgr->Release();
        gPassBoxDocumentMgr = 0;
    }

    if (gPassBoxStore != 0)
    {
        gPassBoxStore->Release();
        gPassBoxStore = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DestroyDisabledDocument()
{
    if (gDisabledDocumentMgr != 0)
    {
        gDisabledDocumentMgr->Pop(TF_POPF_ALL);
        gDisabledDocumentMgr->Release();
        gDisabledDocumentMgr = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CancelComposition()
{
    ComPtr<ITfContext> inputContext;
    V(gTextBoxDocumentMgr->GetBase(&inputContext.Ref()));

    ComPtr<ITfContextOwnerCompositionServices> owner;
    V(inputContext->QueryInterface<ITfContextOwnerCompositionServices>(&owner.Ref()));

    V(owner->TerminateComposition(0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool OnShowKeyboard(void*, UIElement* focusedElement)
{
    NS_ASSERT(gCurrentWindow != 0);

    TextBox* textBox = NsDynamicCast<TextBox*>(focusedElement);
    if (textBox != 0)
    {
        gTextBoxStore->AssociateControl(gCurrentWindow, textBox);

        V(gThreadMgr->SetFocus(gTextBoxDocumentMgr));

        return false;
    }

    PasswordBox* passBox = NsDynamicCast<PasswordBox*>(focusedElement);
    if (passBox != 0)
    {
        gPassBoxStore->AssociateControl(gCurrentWindow, passBox);

        V(gThreadMgr->SetFocus(gPassBoxDocumentMgr));

        return false;
    }

    V(gThreadMgr->SetFocus(gDisabledDocumentMgr));

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void OnHideKeyboard(void*)
{
    V(gThreadMgr->SetFocus(gDisabledDocumentMgr));

    if (gTextBoxStore->IsEnabled())
    {
        CancelComposition();

        gTextBoxStore->AssociateControl(0, 0);
    }
    else if (gPassBoxStore->IsEnabled())
    {
        gPassBoxStore->AssociateControl(0, 0);
    }
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void TSF::Init()
{
    if (gInitCount++ == 0)
    {
        V(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));

        V(CoCreateInstance(CLSID_TF_ThreadMgr, 0, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr,
            (void**)&gThreadMgr));

        V(gThreadMgr->Activate(&gClientId));

        CreateTextBoxDocument();
        CreatePassBoxDocument();
        CreateDisabledDocument();

        ConfigureJapaneseIME();

        GUI::SetSoftwareKeyboardCallbacks(0, &OnShowKeyboard, &OnHideKeyboard);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TSF::Shutdown()
{
    if (gInitCount > 0 && --gInitCount == 0)
    {
        gCurrentWindow = 0;

        GUI::SetSoftwareKeyboardCallbacks(0, 0, 0);

        DestroyTextBoxDocument();
        DestroyPassBoxDocument();
        DestroyDisabledDocument();

        if (gThreadMgr != 0)
        {
            V(gThreadMgr->Deactivate());
            gThreadMgr->Release();
            gThreadMgr = 0;
            gClientId = TF_CLIENTID_NULL;
        }

        CoUninitialize();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TSF::ActivateWindow(void* hWnd)
{
    NS_LOG_TRACE("ActivateWindow(hWnd=%p)", hWnd);
    NS_ASSERT(gCurrentWindow == 0 || gCurrentWindow == hWnd);
    gCurrentWindow = (HWND)hWnd;

    // NOTE: When hWnd is activated, the associated Noesis View automatically restores keyboard
    // focus to the corresponding control. If it is a TextBox, then OnShowKeyboard will be called.
    // But calls to gThreadMgr->SetFocus() inside WM_ACTIVATE seem to have no effect, so we need to
    // do it later. We enqueue a move message and we will call SetFocus() inside TSF::MoveWindow().
    PostMessage(gCurrentWindow, WM_EXITSIZEMOVE, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TSF::DeactivateWindow(void* hWnd)
{
    NS_LOG_TRACE("DeactivateWindow(hWnd=%p)", hWnd);
    NS_ASSERT(gCurrentWindow == hWnd);
    gCurrentWindow = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TSF::MoveWindow(void* hWnd)
{
    NS_LOG_TRACE("MoveWindow(hWnd=%p)", hWnd);
    if (gCurrentWindow == hWnd && gThreadMgr != 0)
    {
        NS_ASSERT(gTextBoxStore != 0);
        if (gTextBoxStore->IsEnabled())
        {
            V(gThreadMgr->SetFocus(gTextBoxDocumentMgr));

            gTextBoxStore->OnLayoutChanged();
        }
        else if (gPassBoxStore->IsEnabled())
        {
            V(gThreadMgr->SetFocus(gPassBoxDocumentMgr));

            gPassBoxStore->OnLayoutChanged();
        }
    }
}
