#pragma once

#include "Base.h"

/* �� ��ǻ�Ϳ� �����ִ� �׷���ī�带 ����� �� �ֵ��� �ʱ�ȣ�Ѵ�. */

/* 1. ����, �׷���ī�带 ���� � ���� �� �� �ֵ��� ����̽��� ��ǥ�ϴ� �� ��ü�� �����ϳ�. */
/* ID3D11Device, ID3D11DeviceContext */

/* 2. ����ü�� ��ü�� �����Ѵ�.(IDXGISwapChain : ����۸� �����. ) */
/* 3. ����� �ؽ��Ŀ� �������ٽ� �ؽ��ĸ� �����Ѵ�. */
/* 4. ���� �ΰ��� �ؽ��ĸ� ��ġ�� ���ε��Ͽ� ���� �� �ؽ��Ŀ� �׸� �� �ֵ��� �ϳ�. */
/* 5. ����Ʈ�� ���� ������ �Ѵ�. */


BEGIN(Engine)

class CGraphic_Device final : public CBase
{
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	/* �׷��� ����̽��� �ʱ�ȭ. */
	HRESULT Initialize(const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice,
		_Inout_ ID3D11DeviceContext** ppContext);

	/* ����۸� �����. */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);

	/* ���̹��� + ���ٽǹ��۸� �����. */
	HRESULT Clear_DepthStencil_View();

	/* �ĸ� ���۸� ������۷� ��ü�Ѵ�.(����۸� ȭ�鿡 ���� �����ش�.) */
	HRESULT Present();

private:		
	// IDirect3DDevice9* == LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext 

	/* �޸� �Ҵ�. (��������, �ε�������, �ؽ��ķε�) �İ�ü�� ������ ���õ� ��Ȱ.  */
	ID3D11Device*				m_pDevice = nullptr;
		
	/* ��ɽ���.(���ε��۾�, �������۸� SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	/* �׸���. Draw */
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;	

	/* �ĸ���ۿ� ������۸� ��ü�ذ��鼭 ȭ�鿡 �����ִ� ��Ȱ. */
	IDXGISwapChain*				m_pSwapChain = nullptr;		


	/* IDirect3DTexture9 */
	/* ID3D11Texture2D : �ؽ��ĸ� ǥ���ϴ� ������ü Ÿ���̴�.
	why? Bind �뵵�� �´� ���������� ����ϱ����� �ؽ��İ�ü�� ����������.  */	


	/* ID3D11ShaderResourceView : ���̴��� ���޵� �� �ִ� �ؽ���Ÿ��. */	
	/* ID3D11RenderTargetView : ����Ÿ�ٿ����� ���� �� �ִ�  �ؽ���Ÿ��. */
	/* ID3D11DepthStencilView : ���̽��ٽǹ��۷μ� ���� �� �ִ� Ÿ��.  */
	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;

	


private:
	/* ����ü�ο��� �ʼ������� �ʿ��� �����ʹ� ����۰� �ʿ��Ͽ�. �Ͽ� ����۸� �����ϱ����� ������ �����ش�. */
	/* ����ü���� ������� == �����(�ؽ���)�� �����ȴ�. */
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	static CGraphic_Device* Create(const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice,
		_Inout_ ID3D11DeviceContext** ppDeviceContextOut);
	virtual void Free() override;
};

END