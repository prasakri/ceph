
#include <ext/rope>
#include "include/types.h"
#include "include/config.h"

#include "Message.h"
#include "Messenger.h"

#include <cassert>
#include <iostream>
using namespace std;



#include "messages/MPing.h"
#include "messages/MOSDRead.h"
#include "messages/MOSDReadReply.h"
#include "messages/MOSDWrite.h"
#include "messages/MOSDWriteReply.h"

#include "messages/MClientRequest.h"
#include "messages/MClientReply.h"

#include "messages/MDirUpdate.h"
#include "messages/MDiscover.h"
#include "messages/MDiscoverReply.h"

#include "messages/MExportDirDiscover.h"
#include "messages/MExportDirDiscoverAck.h"
#include "messages/MExportDirPrep.h"
#include "messages/MExportDirPrepAck.h"
#include "messages/MExportDirWarning.h"
#include "messages/MExportDir.h"
#include "messages/MExportDirNotify.h"
#include "messages/MExportDirNotifyAck.h"
#include "messages/MExportDirFinish.h"

#include "messages/MHeartbeat.h"

#include "messages/MInodeUpdate.h"
#include "messages/MInodeExpire.h"
#include "messages/MDirExpire.h"
#include "messages/MCacheExpire.h"

#include "messages/MLock.h"

Message *
decode_message(crope& ser)
{
  int type;

  // peek at type
  ser.copy(0, sizeof(int), (char*)&type);

  // make message
  Message *m;
  switch(type) {

	// -- with payload --

  case MSG_PING:
	m = new MPing();
	break;

  case MSG_OSD_READ:
	m = new MOSDRead();
	break;

  case MSG_OSD_READREPLY:
	m = new MOSDReadReply();
	break;

  case MSG_OSD_WRITE:
	m = new MOSDWrite();
	break;

  case MSG_OSD_WRITEREPLY:
	m = new MOSDWriteReply();
	break;

	// clients
  case MSG_CLIENT_REQUEST:
	m = new MClientRequest();
	break;

  case MSG_CLIENT_REPLY:
	m = new MClientReply();
	break;

	// mds
  case MSG_MDS_DIRUPDATE:
	m = new MDirUpdate();
	break;

  case MSG_MDS_DISCOVER:
	m = new MDiscover();
	break;
  case MSG_MDS_DISCOVERREPLY:
	m = new MDiscoverReply();
	break;

  case MSG_MDS_EXPORTDIRDISCOVER:
	m = new MExportDirDiscover();
	break;
  case MSG_MDS_EXPORTDIRDISCOVERACK:
	m = new MExportDirDiscoverAck();
	break;

  case MSG_MDS_EXPORTDIR:
	m = new MExportDir();
	break;

  case MSG_MDS_EXPORTDIRFINISH:
	m = new MExportDirFinish();
	break;

  case MSG_MDS_EXPORTDIRNOTIFY:
	m = new MExportDirNotify();
	break;

  case MSG_MDS_EXPORTDIRNOTIFYACK:
	m = new MExportDirNotifyAck();
	break;

  case MSG_MDS_EXPORTDIRPREP:
	m = new MExportDirPrep();
	break;

  case MSG_MDS_EXPORTDIRPREPACK:
	m = new MExportDirPrepAck();
	break;

  case MSG_MDS_EXPORTDIRWARNING:
	m = new MExportDirWarning();
	break;

  case MSG_MDS_HEARTBEAT:
	m = new MHeartbeat();
	break;

  case MSG_MDS_CACHEEXPIRE:
	m = new MCacheExpire();
	break;

  case MSG_MDS_INODEUPDATE:
	m = new MInodeUpdate();
	break;

  case MSG_MDS_INODEEXPIRE:
	m = new MInodeExpire();
	break;

  case MSG_MDS_DIREXPIRE:
	m = new MDirExpire();
	break;

  case MSG_MDS_LOCK:
	m = new MLock();
	break;


	// -- simple messages without payload --

  case MSG_MDS_SHUTDOWNSTART:
  case MSG_MDS_SHUTDOWNFINISH:
  case MSG_CLIENT_DONE:
	m = new Message(type);
	break;

  default:
	dout(1) << "can't decode unknown message type " << type << endl;
	assert(0);
  }
  
  // decode envelope
  crope env = ser.substr(0, MSG_ENVELOPE_LEN);
  m->decode_envelope(env);

  // payload
  if (ser.length() > MSG_ENVELOPE_LEN) {
	crope payload = ser.substr(MSG_ENVELOPE_LEN,
							   ser.length() - MSG_ENVELOPE_LEN);
	m->decode_payload(payload);
  }

  // done!
  return m;
}
