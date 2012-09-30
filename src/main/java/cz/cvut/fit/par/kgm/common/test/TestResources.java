package cz.cvut.fit.par.kgm.common.test;

import java.io.IOException;
import java.net.URL;
import java.nio.charset.Charset;

import com.google.common.base.Charsets;
import com.google.common.io.Resources;

/**
 * TODO
 * 
 * @author stoupa
 */
public class TestResources {

	public static String utf8ResourceAsString( String classpathResource ) throws IOException {
		URL res = Resources.getResource( classpathResource );
		return Resources.toString( res, Charsets.UTF_8 );
	}
	
	public static String resourceAsString( String classpathResource, Charset charset ) throws IOException {
		URL res = Resources.getResource( classpathResource );
		return Resources.toString( res, charset );
	}
	
}
