#pragma once
// https://stackoverflow.com/questions/27716779/crash-when-safe-release-is-called-on-immdeviceenumerator
#include <mmdeviceapi.h>
#include <stdio.h>
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }
class CMMNotificationClient :
	public IMMNotificationClient
{
	LONG _cRef;
	IMMDeviceEnumerator *_pEnumerator;

	// Private function to print device-friendly name
	HRESULT _PrintDeviceName(LPCWSTR  pwstrId);

public:
	CMMNotificationClient() :
		_cRef(1),
		_pEnumerator(NULL)
	{
		
	}

	~CMMNotificationClient()
	{
		SAFE_RELEASE(_pEnumerator)
	}

	VOID RegisterMessage(UINT msg);
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postmessagea
	// https://docs.microsoft.com/nl-nl/windows/win32/winmsg/wm-user?redirectedfrom=MSDN
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerwindowmessagea
	// IUnknown methods -- AddRef, Release, and QueryInterface

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&_cRef);
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		ULONG ulRef = InterlockedDecrement(&_cRef);
		if (0 == ulRef)
		{
			delete this;
		}
		return ulRef;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid, VOID **ppvInterface)
	{
		if (IID_IUnknown == riid)
		{
			AddRef();
			*ppvInterface = (IUnknown*)this;
		}
		else if (__uuidof(IMMNotificationClient) == riid)
		{
			AddRef();
			*ppvInterface = (IMMNotificationClient*)this;
		}
		else
		{
			*ppvInterface = NULL;
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	// Callback methods for device-event notifications.

	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		EDataFlow flow, ERole role,
		LPCWSTR pwstrDeviceId)
	{
		char  *pszFlow = "?????";
		char  *pszRole = "?????";

		_PrintDeviceName(pwstrDeviceId);

		switch (flow)
		{
		case eRender:
			pszFlow = "eRender";
			break;
		case eCapture:
			pszFlow = "eCapture";
			break;
		}

		switch (role)
		{
		case eConsole:
			pszRole = "eConsole";
			break;
		case eMultimedia:
			pszRole = "eMultimedia";
			break;
		case eCommunications:
			pszRole = "eCommunications";
			break;
		}

		printf("  -->New default device: flow = %s, role = %s\n",
			pszFlow, pszRole);
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
	{
		_PrintDeviceName(pwstrDeviceId);

		printf("  -->Added device\n");
		return S_OK;
	};

	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
	{
		_PrintDeviceName(pwstrDeviceId);

		printf("  -->Removed device\n");
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
		LPCWSTR pwstrDeviceId,
		DWORD dwNewState)
	{
		char  *pszState = "?????";

		_PrintDeviceName(pwstrDeviceId);

		switch (dwNewState)
		{
		case DEVICE_STATE_ACTIVE:
			pszState = "ACTIVE";
			break;
		case DEVICE_STATE_DISABLED:
			pszState = "DISABLED";
			break;
		case DEVICE_STATE_NOTPRESENT:
			pszState = "NOTPRESENT";
			break;
		case DEVICE_STATE_UNPLUGGED:
			pszState = "UNPLUGGED";
			break;
		}

		printf("  -->New device state is DEVICE_STATE_%s (0x%8.8x)\n",
			pszState, dwNewState);

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
		LPCWSTR pwstrDeviceId,
		const PROPERTYKEY key)
	{
		_PrintDeviceName(pwstrDeviceId);

		printf("  -->Changed device property "
			"{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x}#%d\n",
			key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3,
			key.fmtid.Data4[0], key.fmtid.Data4[1],
			key.fmtid.Data4[2], key.fmtid.Data4[3],
			key.fmtid.Data4[4], key.fmtid.Data4[5],
			key.fmtid.Data4[6], key.fmtid.Data4[7],
			key.pid);
		return S_OK;
	}
};

