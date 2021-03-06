/*
 * Copyright (C) 2001-2008 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "stdinc.h"
#include "DCPlusPlus.h"

#include "SSLSocket.h"
#include "LogManager.h"
#include "SettingsManager.h"

#include <openssl/err.h>

namespace dcpp {

SSLSocket::SSLSocket(SSL_CTX* context) throw(SocketException) : ctx(context), ssl(0) {

}

void SSLSocket::connect(const string& aIp, uint16_t aPort) throw(SocketException) {
	Socket::setBlocking(true);
	Socket::connect(aIp, aPort);

	ssl.reset(SSL_new(ctx));
	if(!ssl)
		checkSSL(-1);

	checkSSL(SSL_set_fd(ssl, sock));
	checkSSL(SSL_connect(ssl));
	dcdebug("Connected to SSL server using %s\n", SSL_get_cipher(ssl));
	Socket::setBlocking(false);
}

void SSLSocket::accept(const Socket& listeningSocket) throw(SocketException) {
	Socket::accept(listeningSocket);

	ssl.reset(SSL_new(ctx));
	if(!ssl)
		checkSSL(-1);

	checkSSL(SSL_set_fd(ssl, sock));
	/// @todo fix blocking if accept fails
	checkSSL(SSL_accept(ssl));
	dcdebug("Connected to SSL client using %s\n", SSL_get_cipher(ssl));
}

int SSLSocket::read(void* aBuffer, int aBufLen) throw(SocketException) {
	if(!ssl) {
		return -1;
	}
	int len = checkSSL(SSL_read(ssl, aBuffer, aBufLen));

	if(len > 0) {
		stats.totalDown += len;
		//dcdebug("In(s): %.*s\n", len, (char*)aBuffer);
	}
	return len;
}

int SSLSocket::write(const void* aBuffer, int aLen) throw(SocketException) {
	if(!ssl) {
		return -1;
	}
	int ret = checkSSL(SSL_write(ssl, aBuffer, aLen));
	if(ret > 0) {
		stats.totalUp += ret;
		//dcdebug("Out(s): %.*s\n", ret, (char*)aBuffer);
	}
	return ret;
}

int SSLSocket::checkSSL(int ret) throw(SocketException) {
	if(!ssl) {
		return -1;
	}
	if(ret <= 0) {
		int err = SSL_get_error(ssl, ret);
		switch(SSL_get_error(ssl, ret)) {
			case SSL_ERROR_NONE:		// Fallthrough - YaSSL doesn't for example return an openssl compatible error on recv fail
			case SSL_ERROR_WANT_READ:	// Fallthrough
			case SSL_ERROR_WANT_WRITE:
				return -1;
			default:
				{
					ssl.reset();
					// @todo replace 80 with MAX_ERROR_SZ or whatever's appropriate for yaSSL in some nice way...
					char errbuf[80];
					throw SocketException(string("SSL Error: ") + ERR_error_string(err, errbuf) + " (" + Util::toString(ret) + ", " + Util::toString(err) + ")"); // @todo Translate
				}
		}
	}
	return ret;
}

int SSLSocket::wait(uint32_t millis, int waitFor) throw(SocketException) {
	if(ssl && (waitFor & Socket::WAIT_READ)) {
		/** @todo Take writing into account as well if reading is possible? */
		char c;
		if(SSL_peek(ssl, &c, 1) > 0)
			return WAIT_READ;
	}
	return Socket::wait(millis, waitFor);
}

bool SSLSocket::isTrusted() const throw() {
	if(!ssl) {
		return false;
	}

	if(SSL_get_verify_result(ssl) != SSL_ERROR_NONE) {
		return false;
	}

	if(!SSL_get_peer_certificate(ssl)) {
		return false;
	}

	return true;
}

std::string SSLSocket::getCipherName() const throw() {
	if(!ssl)
		return Util::emptyString;
	
	return SSL_get_cipher_name(ssl);
}

std::string SSLSocket::getDigest() const throw() {
	if(!ssl)
		return Util::emptyString;
	X509* x509 = SSL_get_peer_certificate(ssl);
	if(!x509)
		return Util::emptyString;
	
	return ssl::X509_digest(x509, EVP_sha1());
}

void SSLSocket::shutdown() throw() {
	if(ssl)
		SSL_shutdown(ssl);
}

void SSLSocket::close() throw() {
	if(ssl) {
		ssl.reset();
	}
	Socket::shutdown();
	Socket::close();
}

} // namespace dcpp
