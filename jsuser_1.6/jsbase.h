#pragma once

namespace v8{
	void LoadGDI(Handle<Object>& glb);
	void LoadKernel(Handle<Object>& glb);
	void LoadFile(Handle<Object>& glb);
	void LoadXml(Handle<Object>& glb);
	void LoadRegistry(Handle<Object>& glb);
	void LoadSocket(Handle<Object>& glb);
	void LoadNet(Handle<Object>& glb);
	void LoadBase();
	LPCWSTR RequireBase(LPCWSTR modelist);
}