#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

//log 名稱
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main(int argc, char *argv[])
{
	//設定command視窗
	CommandLine cmd;
	cmd.Parse (argc, argv);

	//log 設定
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	//新增node
	NodeContainer nodes;
	nodes.Create (2);//node.Get(0), node.Get(1)

	//設置裝置速度,延遲速度
	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

	//將節點的設定安裝到裝置上面
	NetDeviceContainer devices;
	devices = pointToPoint.Install (nodes);

	//使用Helper將網卡安裝到節點
	InternetStackHelper stack;
	stack.Install (nodes);

	//設定節點的IP位址,遮罩
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign (devices);

	//針對node安裝服務
	//設定server-UDP連線，port:9
	UdpEchoServerHelper echoServer(9);

	//serverApps設定為節點1
	ApplicationContainer serverApps = echoServer.Install (nodes.Get(1));
	//服務開啟,結束
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));


	//client設定連線ip_address:node1, port:9
	UdpEchoClientHelper echoClient (interfaces.GetAddress(1), 9);
	//最大接收封包為1
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	//間隔時間1.0s
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	//封包大小1024bytes
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));


	//clientApps設定為節點1
	ApplicationContainer clientApps = echoClient.Install (nodes.Get(0));

	//服務開啟,結束
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	//https://www.youtube.com/watch?v=CWJRkO9EB5s
	//http://zhua0404.blogspot.com/2017/09/ns3-example-1-4-tracing-system.html
	//Pcap Tracing
	pointToPoint.EnablePcapAll ("myfirst");

	//模擬器啟用
	Simulator::Run ();
	Simulator::Destroy ();

	return 0;
}