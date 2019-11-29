# Socket Programming作業格式


## 環境

- 使用Virtual Box 6.0版本
	- 虛擬機網路利用橋接介面卡(bridge)連接
- Server環境
	- Ubuntu 16.04 server版
		- ipv4: 192.168.43.18
- Client環境
	- Kali 2018.04
		- ipv4: 192.168.43.26
	- Ubuntu 18.04
		- ipv4: 192.168.43.140


1. Client/Srever程式(1個Client對1個Server)
- 說明
	- 右側為Ubuntu16.04當作server(使用ssh連線進虛擬機)
		- server ip: 192.168.43.18/port 4444
	- 左側為Ubuntu18.04當作client(本機terminal)
		- client ip: 192.168.43.140/port 4444

	- test.txt(server)內容, get.txt(client拿的)內容
		- `Here is the Flag La~~~~~~~~~~~~~~`
	- 過程
		- client向server請求資料
		- server去本機拿test.txt進行串流
		- client開檔(get.txt)準備接收
		- accept後開始進行傳輸
		- 傳完之後直接將結果print出

2. Client/Srever程式(多個Client對1個Server)(30%)

- 說明
	- 右側為Ubuntu16.04當作server(使用ssh連線進虛擬機)
		- server ip: 192.168.43.18/port 4444
	- 左上(client)與上題一樣，當作第一台client
		- client1 ip: 192.168.43.140/port4444
	- 左下為Kali 2018.04當作第二台client
		- client2 ip: 192.168.43.26/port 4444

	- test.txt(server)內容, get.txt(ubuntu16.04/client1), get2.txt(kali/client2拿的)內容
		- `Here is the Flag La~~~~~~~~~~~~~~`
	- 過程
		- 過程與前題相同
		- 兩邊交互請求時仍然可以回傳
		- 傳完之後直接將結果print出



