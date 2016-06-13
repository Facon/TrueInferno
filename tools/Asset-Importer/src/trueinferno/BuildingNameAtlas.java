package trueinferno;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map.Entry;
import java.util.Properties;

public class BuildingNameAtlas {
	private HashMap<String,String> given2Real;
	private HashMap<String,String> real2Given;
	private HashSet<String> realNames;
	private HashSet<String> givenNames;
	
	public BuildingNameAtlas(){
		given2Real = new HashMap<String,String>();
		real2Given = new HashMap<String,String>();
		realNames = new HashSet<String>();
		givenNames = new HashSet<String>();
	}
	
	public void load(Properties properties) {
		for(Entry<Object, Object> e : properties.entrySet()){
			String realName = (String) e.getKey();
			String givenName = (String) e.getValue();
			
			given2Real.put(givenName, realName);
			real2Given.put(realName, givenName);
			realNames.add(realName);
			givenNames.add(givenName);
		}
	}
	
	public String givenName2RealName(String givenName){
		return given2Real.get(givenName);
	}
	
	public String realName2GivenName(String realName){
		return real2Given.get(realName);
	}
	
	@Override
	public String toString() {
		return 
				"BuildingNameAtlas [given2Game=" + given2Real + "\n" +
				"real2Given=" + real2Given + "\n" +
				"realNames=" + realNames + "\n" +
				"givenNames=" + givenNames
				+ "]";
	}

	public HashSet<String> getRealNames() {
		return realNames;
	}

	public HashSet<String> getGivenNames() {
		return givenNames;
	}

}
