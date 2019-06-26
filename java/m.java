import com.alibaba.fastjson.*;
import org.apache.commons.lang3.*;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;
import java.util.regex.Pattern;

import static com.remobile.moment.Moment.moment;

public class m {
    public static <T> T ifNull(T value, T defaultValue) {
        return null == value ? defaultValue : value;
    }

    public static Date date(String text) {
        if (null == text) {
            return null;
        }
        return moment(text).toDate();
    }

    public static Boolean test(String str, String reg) {
        if (null == str) {
            return false;
        }
        return Pattern.compile(reg).matcher(str).find();
    }
    public static <T> List<T> list(T... args) {
        List<T> list = new ArrayList();
        for (T arg : args) {
            list.add(arg);
        }
        return list;
    }

    public static Object object(Object... args) {
        Map context = new HashMap();
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            context.put(args[i], args[i + 1]);
        }
        return context;
    }

    public static JSONObject json(Object... args) {
        JSONObject context = new JSONObject();
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            context.put((String) args[i], args[i + 1]);
        }
        return context;
    }

    public static Map object(Object obj) {
        Map context = new HashMap();
        try {
            Class cls = (Class) obj.getClass();
            Field[] fields = cls.getDeclaredFields();
            for (Field field : fields) {
                field.setAccessible(true);
                context.put(field.getName(), field.get(obj));
            }
            return context;
        } catch (Exception e) {
        }
        return context;
    }

    public static Map merge(Map map, Object... args) {
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }

    public static Map merge(Object object, Object... args) {
        Map map = object(object);
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }

    public static Map omit(Map map, String... args) {
        for (String arg : args) {
            map.remove(arg);
        }
        return map;
    }

    public static <T> List<T> concat(List<T> list, List<T> list1) {
        for (T item : list1) {
            list.add(item);
        }
        return list;
    }

    public static Object get(Object obj, String key) {
        try {
            Field field = obj.getClass().getDeclaredField(key);
            field.setAccessible(true);
            return field.get(obj);
        } catch (Exception e) {
            System.out.println(e);
        }
        return null;
    }

    public static <T, M> List<T> pick(List<M> list, String key) {
        List<T> l = new ArrayList<T>();
        for (M item : list) {
            T t = (T) get(item, key);
            if (null != t) {
                l.add(t);
            }
        }
        return l;
    }

    public static Boolean eq(String str1, String str2) {
        if (null == str1) {
            str1 = "";
        }
        if (null == str2) {
            str2 = "";
        }
        return str1.equals(str2);
    }

    public static String extname(String path) {
        int index = path.lastIndexOf(".");
        if (-1 == index) {
            return "";
        }
        return path.substring(index);
    }

    public static Boolean isMedia(String url) {
        return test(url, "^/[a-z0-9]{24}(\\.[^/]+)?");
    }

    public static Boolean _isMediaItem(String url) {
        return test(url, "^/[a-z0-9]{24}(\\.[^/]+)?$");
    }

    public static String getMediaId(String url) {
        return url.replaceAll("^/([a-z0-9]{24})(\\.[^/]+)?$", "$1");
    }

    public static List<String> _getMediaIdList (List<String> list, String line) {
        if (null == line) {
            return list;
        }
        String[] urls = line.split(",");
        for (String url : urls) {
            if (_isMediaItem(url)) {
                list.add(getMediaId(url));
            }
        }
        return list;
    }

    public static String _getMediaName(String url) {
        if (null == url) {
            return null;
        }
        String extname = extname(url);
        String id = url.replaceAll("http://localhost:3000", "").replaceAll(extname, "");
        if (test(id, "^/[a-z0-9]{24}$")) {
            return id + extname;
        }
        return url;
    }

    public static String getMediaName(String url) {
        if (null == url) {
            return null;
        }
        if (test(url, ",")) {
            String[] urls = url.split(",");
            for (int i = 0; i < urls.length; i++) {
                urls[i] = getMediaName(urls[i]);
            }
            return StringUtils.join(urls, ",");
        }
        return _getMediaName(url);
    }

    public static String getMediaUrl(String name) {
        if (null == name) {
            return null;
        }
        String urls[] = name.split(",");
        for (int i = 0; i < urls.length; i++) {
            urls[i] = isMedia(urls[i]) ? "http://localhost:3000" + urls[i] : urls[i];
        }
        return StringUtils.join(urls, ",");
    }

    public static String getDefaultPassWord(String... args) {
        if (null == args[1]) {
            return args[0].substring(5);
        }
        return args[1];
    }

    public static void incRef(Object... args) {
        List<String> list = new ArrayList<>();
        for (Object arg : args) {
            _getMediaIdList(list, (String) arg);
        }
        System.out.println(list);
    }

    public static void replaceRef(Object... args) {
        List<String> incList = new ArrayList<>();
        List<String> decList = new ArrayList<>();

        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            if (!eq((String)args[i], "")) {
                _getMediaIdList(incList, (String) args[i]);
                _getMediaIdList(decList, (String) args[i+i]);
            }
        }
        System.out.println(incList);
        System.out.println(decList);
    }

    public static void decRef(Object... args) {
        List<String> list = new ArrayList<>();
        for (Object arg : args) {
            if (arg instanceof List) {
                for (String url : (List<String>) arg) {
                    _getMediaIdList(list, url);
                }
            } else {
                _getMediaIdList(list, (String) arg);
            }
        }
        System.out.println(list);
    }
}
