package trueinferno;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;

public class ImportMaterialTask extends Task {

	public enum MaterialTaskOption {
		IMPORT_CREATE_DIFFUSE,
		IMPORT_CREATE_NO_TEXTURE,
		SKIP
	}
	
	/** Extensiones válidas de un fichero de material */
	private static final String[] MATERIAL_EXTENSIONS = {"material"};
	
	/** Extensiones válidas de un fichero de textura */
	private static final String[] TEXTURE_EXTENSIONS = {"jpg","png"};
	
	private String realName;
	private String givenName;
	private boolean result;
	private String observations;

	private File inputModelDir;
	private File outputMaterialDir;
	private File outputTextureDir;
	private File mapFile;

	// Filtro de ficheros que acepta ficheros de textura de Albedo (difuso)
	private IOFileFilter isAlbedoFileFilter;

	private MaterialTaskOption materialTaskOption;
	
	public ImportMaterialTask(String realName, String givenName, File inputModelDir, File outputMaterialDir, File outputTextureDir, File mapFile, MaterialTaskOption materialTaskOption) {
		this.realName = realName;
		this.givenName = givenName;
		this.inputModelDir = inputModelDir;
		this.outputMaterialDir = outputMaterialDir;
		this.outputTextureDir = FileUtils.getFile(outputTextureDir, realName);
		this.mapFile = mapFile;
		this.materialTaskOption = materialTaskOption;
		
		isAlbedoFileFilter = 
				FileFilterUtils.asFileFilter(
						new FileFilter(){
							@Override
							public boolean accept(File pathname) {
								String extension = FilenameUtils.getExtension(pathname.getName()).toLowerCase();
								String name = pathname.getName().toLowerCase();

								return Arrays.asList(TEXTURE_EXTENSIONS).contains(extension) && name.matches(".*(albedo|diff?us).*"); 
							}
						}
						);				
	}

	@Override
	public String getTitle() {
		return "ImportMaterial";
	}

	@Override
	public String getDescription() {
		return realName;
	}

	@Override
	public boolean getResult() {
		return result;
	}

	@Override
	public String getObservations() {
		return observations;
	}

	@Override
	public void execute() {
		try{
			importMaterial();
			result = true;
			observations = "";
		}
		catch(Exception e){
			//Logger.getGlobal().log(Level.INFO, "Error in " + getTitle() + " - " + getDescription(), e);
			System.out.println("Error in " + getTitle() + " - " + getDescription());
			e.printStackTrace();
			result = false;
			observations = e.getMessage();
		}
	}

	private void importMaterial() throws TrueInfernoException, IOException {
		// Buscamos el fichero con el material
		File materialFile = null;
		try{
			materialFile = findMaterialFile();
		}
		
		// Si no se encontró el asset del material lo creamos directamente
		catch (AssetFileNotFoundException e) {
			materialFile = createMaterialFile();
		}
		
		// Copiamos el material
		Util.copyToDirWithBackup(materialFile, outputMaterialDir);
		
		// Obtenemos el nombre del material (que NO es el nombre del fichero)
		String materialName = getMaterialName(materialFile);
		
		// Actualizamos el mapa
		updateAttributeInMap(mapFile, realName, "material", materialName);
	}

	private String getMaterialName(File materialFile) throws IOException, TrueInfernoException {
		String content = FileUtils.readFileToString(materialFile, (Charset)null);
		Pattern p = Pattern.compile("^.*material\\s+(\\S+)$", Pattern.MULTILINE);
		Matcher m = p.matcher(content);
		
		if(!m.find())
			throw new TrueInfernoException("No material name found in material script file '"+materialFile+"'");
			
		String name = m.group(1);
		
		if(m.find())
			throw new TrueInfernoException("Multiple names found in material script file '"+materialFile+"'");
		
		return name;
	}

	private File createMaterialFile() throws IOException, TrueInfernoException {
		switch(materialTaskOption){
		case IMPORT_CREATE_DIFFUSE:
			return createDiffuseMaterialFile();

		case IMPORT_CREATE_NO_TEXTURE:
			return createNoTextureMaterialFile();

		default:
			throw new TrueInfernoException("There is no createMaterialFile implementation for materialTaskOption="+materialTaskOption);
		}
	}
	
	private File createDiffuseMaterialFile() throws IOException, TrueInfernoException {
		// Localizamos el fichero de origen con la textura de Albedo 
		File diffuseTextureFile = findAlbedoTextureFile();
		
		// Lo ponemos en su carpeta
		Util.copyToDirWithBackup(diffuseTextureFile, outputTextureDir);
		
		// Creamos un nuevo fichero de material que haga uso de la textura de difuso obtenida
		
		File newMaterialFile = new File("tmp" + File.separator + realName + ".material");
		
		ArrayList<String> lines = new ArrayList<String>();
		lines.add("material "+realName);
		lines.add("{");
		lines.add("	technique");
		lines.add("	{");
		lines.add("		pass");
		lines.add("		{");
		lines.add("	");
		lines.add("			texture_unit");
		lines.add("			{");
		lines.add("				texture building/"+realName+"/"+diffuseTextureFile.getName());
		lines.add("			}");
		lines.add("		");
		lines.add("		}");
		lines.add("	}");
		lines.add("}");
		
		FileUtils.writeLines(newMaterialFile, lines, false);

		return newMaterialFile;
	}

	private File createNoTextureMaterialFile() throws IOException, TrueInfernoException {
		// Creamos un nuevo fichero de material que haga uso de la textura de difuso obtenida
		File newMaterialFile = new File("tmp" + File.separator + realName + ".material");
		
		ArrayList<String> lines = new ArrayList<String>();
		lines.add("material "+realName);
		lines.add("{");
		lines.add("	technique");
		lines.add("	{");
		lines.add("		pass");
		lines.add("		{");
		lines.add("			ambient  1 1 1 1");
		lines.add("			diffuse  0.4 0.4 0.4 1");
		lines.add("			specular 1 1 1 30");
		lines.add("			emissive 0 0 0 1");
		lines.add("		}");
		lines.add("	}");
		lines.add("}");
		
		FileUtils.writeLines(newMaterialFile, lines, false);

		return newMaterialFile;
	}
	
	private File findMaterialFile() throws TrueInfernoException {
		File dir = FileUtils.getFile(inputModelDir, givenName);
		
		Collection<File> files = FileUtils.listFiles(dir, MATERIAL_EXTENSIONS, true);
		if(files.size() > 1)
			throw new TrueInfernoException("Multiple material files found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"': "+files);
		else if (files.isEmpty())
			throw new AssetFileNotFoundException("No material file found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"'");
		
		return files.iterator().next();
	}
	
	private File findAlbedoTextureFile() throws TrueInfernoException {
		File dir = FileUtils.getFile(inputModelDir, givenName);
		
		Collection<File> files = FileUtils.listFiles(dir, isAlbedoFileFilter, TrueFileFilter.TRUE);
		if(files.size() > 1)
			throw new TrueInfernoException("Multiple texture files found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"': "+files);
		else if (files.isEmpty())
			throw new AssetFileNotFoundException("No texture file found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"'");
		
		return files.iterator().next();
	}
	
}
