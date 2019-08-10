import com.alibaba.fastjson.*;
import org.apache.commons.lang3.*;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import com.github.underscore.lodash.U;
import java.util.*;
import java.util.regex.Pattern;

import static com.remobile.moment.Moment.moment;

public class m {
    // 默认值
    public static <T> T ifNull(T value, T defaultValue) {
        return null == value ? defaultValue : value;
    }

    // 字符串转日期
    public static Integer i(String s) {
        return Integer.valueOf(s);
    }

    // 字符串转日期
    public static Date date(String text) {
        if (eq(text, null)) {
            return null;
        }
        return moment(text).toDate();
    }

    // 正则表达式检测
    public static Boolean test(String str, String reg) {
        if (null == str) {
            return false;
        }
        return Pattern.compile(reg).matcher(str).find();
    }

    // 判断是否是自然数
    public static Boolean isNumber(String str) {
        return test(str, "^0|[1-9][0-9]*$");
    }


    // 生成一个List
    public static <T> List<T> list(T... args) {
        List<T> list = new ArrayList();
        for (T arg : args) {
            list.add(arg);
        }
        return list;

    }

    // 生成一个Map
    public static Object object(Object... args) {
        Map context = new HashMap();
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            context.put(args[i], args[i + 1]);
        }
        return context;
    }

    // 生成一个JSON
    public static JSONObject json(Object... args) {
        JSONObject context = new JSONObject();
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            context.put((String) args[i], args[i + 1]);
        }
        return context;
    }

    // 将类转化为Map
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

    // 将Map合并到Map
    public static Map merge(Map map, Object... args) {
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }

    // 将Map合并到Object
    public static Map merge(Object object, Object... args) {
        Map map = object(object);
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }

    // 去除Map中的某些值
    public static Map omit(Map map, String... args) {
        for (String arg : args) {
            map.remove(arg);
        }
        return map;
    }

    // 连接两个list
    public static <T> List<T> concat(List<T> list, List<T> list1) {
        for (T item : list1) {
            list.add(item);
        }
        return list;
    }

    // 通过key值获取Object的值
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

    // 提取Collections中的某一个值
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

    // split
    public static List<String> split(String val, String del) {
        if (isNull(val)) {
            return new ArrayList<String>();
        }
        return Arrays.asList(val.split(del));
    }

    // 判断字符串相等
    public static Boolean eq(String str1, String str2) {
        if (null == str1) {
            str1 = "";
        }
        if (null == str2) {
            str2 = "";
        }
        return str1.equals(str2);
    }

    // 判断字符串是否是空
    public static Boolean isNull(String str) {
        return null == str || str.equals("");
    }

    // 判断字符串是否是空
    public static String toString(Object obj) {
        return null == obj ? "null" : obj.toString();
    }

    // 获取文件扩展名
    public static String extname(String path) {
        int index = path.lastIndexOf(".");
        if (-1 == index) {
            return "";
        }
        return path.substring(index);
    }


    // 判断url是否是media
    public static Boolean isMedia(String url) {
        return test(url, "^/[a-z0-9]{24}(\\.[^/]+)?");
    }

    // 内部函数 判断url是否是完整的media
    public static Boolean _isMediaItem(String url) {
        return test(url, "^/[a-z0-9]{24}(\\.[^/]+)?$");
    }

    // 从url获取media的Id
    public static String getMediaId(String url) {
        return url.replaceAll("^/([a-z0-9]{24})(\\.[^/]+)?$", "$1");
    }

    // 内部函数 获取media的Id数组
    public static List<String> _getMediaIdList (List<String> list, String line) {
        if (isNull(line)) {
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

    // 内部函数
    public static String _getMediaName(String url) {
        if (isNull(url)) {
            return null;
        }
        String extname = extname(url);
        String id = url.replaceAll("http://localhost:3000", "").replaceAll(extname, "");
        if (test(id, "^/[a-z0-9]{24}$")) {
            return id + extname;
        }
        return url;
    }

    // 从url获取meida的名称
    // getMediaName("url", "url, url")
    public static String getMediaName(String url) {
        if (isNull(url)) {
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

    // 通过meida的名称获取url
    public static String getMediaUrl(String name) {
        if (isNull(name)) {
            return null;
        }
        String urls[] = name.split(",");
        for (int i = 0; i < urls.length; i++) {
            urls[i] = isMedia(urls[i]) ? "http://localhost:3000" + urls[i] : urls[i];
        }
        return StringUtils.join(urls, ",");
    }

    // 获取默认密码
    public static String getDefaultPassWord(String... args) {
        if (isNull(args[1])) {
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

    // 通过角色和实际的权限获取存入到数据库的权限值
    public static String getDatabaseAuthority(String roleAuthority, String authority) {
        List<String> roleList = m.split(roleAuthority, ",");
        List<String> list = m.split(authority, ",");
        List<String> add = U.difference(list, roleList);
        List<String> sub = U.map(U.difference(roleList, list), item -> "-" + item);
        return U.join(U.concat(sub, add),",");
    }

    // 通过角色和数据库的权限获取存入到真实的权限值
    public static String getRealAuthority(String roleAuthority, String authority) {
        List<String> roleList = m.isNull(roleAuthority) ? new ArrayList<>() : m.split(roleAuthority, ",");
        List<String> list = m.isNull(authority) ? new ArrayList<>() : m.split(authority, ",");
        List<String> add = U.reject(list, o -> o.startsWith("-"));
        List<String> sub = U.map(U.filter(list, o -> o.startsWith("-")), m -> m.substring(1));
        List<String> roleAdd = U.reject(roleList, o -> U.contains(sub, o));
        return U.join(U.concat(roleAdd, add),",");
    }
}
