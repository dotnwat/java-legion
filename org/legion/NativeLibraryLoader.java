package org.legion;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

public class NativeLibraryLoader {
  //singleton
  private static final NativeLibraryLoader instance = new NativeLibraryLoader();
  private static boolean initialized = false;

  private static final String sharedLibraryName = "liblegion_jni.so";
  private static final String tempFilePrefix = "liblegion_jni";
  private static final String tempFileSuffix = ".so";

  /**
   * Get a reference to the NativeLibraryLoader
   *
   * @return The NativeLibraryLoader
   */
  public static NativeLibraryLoader getInstance() {
    return instance;
  }

  /**
   * Attempts to extract the native RocksDB library
   * from the classpath and load it
   *
   * @param tmpDir A temporary directory to use
   *   to copy the native library to. If null,
   *   or the empty string, we rely on Java's
   *   {@link java.io.File#createTempFile(String, String)}
   *   function to provide a temporary location.
   *   The temporary file will be registered for deletion
   *   on exit.
   *
   * @throws java.io.IOException if a filesystem operation fails.
   */
  public synchronized void loadLibraryFromJar(final String tmpDir) throws IOException {
    if (!initialized) {
      final File temp;
      if (tmpDir == null || tmpDir.equals("")) {
        temp = File.createTempFile(tempFilePrefix, tempFileSuffix);
      } else {
        temp = new File(tmpDir, sharedLibraryName);
      }

      if (!temp.exists()) {
        throw new RuntimeException("File " + temp.getAbsolutePath() + " does not exist.");
      } else {
        temp.deleteOnExit();
      }

      // attempt to copy the library from the Jar file to the temp destination
      try (final InputStream is = getClass().getClassLoader().
          getResourceAsStream(sharedLibraryName)) {
        if (is == null) {
          throw new RuntimeException(sharedLibraryName + " was not found inside JAR.");
        } else {
          Files.copy(is, temp.toPath(), StandardCopyOption.REPLACE_EXISTING);
        }
      }

      System.load(temp.getAbsolutePath());
      initialized = true;
    }
  }

  /**
   * Private constructor to disallow instantiation
   */
  private NativeLibraryLoader() {
  }
}
