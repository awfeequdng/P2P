/*--------------------------------------------------------------------
　　　/\~~~~~~~~~~~~~\　　　��　　^*^　　　℃　　$$　 .℃ 
　　./　\~~~��~　 ~~~~\ ＃　　淵貴 .酔赤　 *　 $����$　 * 
　　/ ^^ \ �T�T�T�T�T�T\.＃　　　 *　*　　*　 $��￥��$　 * 
　..��[]　��弥　弥 �� |�А堯� .　　　　　*　 $��＊＊��$　 * 
　&&��　　��　　　 ��'|'�� @　　　　　　　* $��＊＊＊��$ * 
�� ＊＊＊＊＊＊＊＊＊＊◇�{�z�y�x垳低淵貴酔赤�颱�||�颱� 


				園咎 ucp2p 垂
井云��1.0
扮寂��2015.08.01
恬宀��Faker

--------------------------------------------------------------------*/



1.	俶勣廣吭議頁��ucp2p 垂卆正噐凪麿垂��萩鳩範凪万垂厮将芦廾挫
	人薩極 卆正議垂嗤�� pthread openssl json
			-lpthread -lssl -lcrypto -lm -ljson
	捲暦匂卆正議垂嗤��	pthread openssl json sqlclient
			-lpthread -lssl -lcrypto -lm -ljson -lmysqlclient
	
	泌惚短嗤芦廾挫斤哘議lib垂��萩峇佩��
	apt-get install openssl-dev
	apt-get install libjson0

	芦廾mysql 方象垂。醤悶芦廾歌深為業
	1. sudo apt-get install mysql-server
　　2. apt-get install mysql-client
　　3. sudo apt-get install libmysqlclient-dev

2.	鳩範侭嗤垂脅芦廾挫朔��峇佩 make 軸辛

3.	泌惚短嗤吭翌議三��園咎宥狛朔氏壓輝念朕村和伏撹和中叱倖猟周

	p2p_client ��
		P2P議酒汽人薩極
	p2p_server ��
		P2P議酒汽捲暦匂殻會
	
	libp2p-client.so ��
		P2P 人薩極卆正議強蓑垂
	
	libp2p-server.so ��
		P2P 捲暦匂極卆正議強蓑垂

4	委 libp2p-client.so libp2p-server.so 申唄欺 /usr/lib/ 和中

